#include "common.h"
#include "Timer.h"
#include "Packet.h"
#include "PacketScheduler.h"
#include "Scheduler.h"
#include "Topology.h"
#include "Node.h"
#include "FIFONode.h"

PacketQueue::PacketQueue(int sz,
                         Address s,
                         Address d)
{
    pending_send = 0;
    max_size = sz;
    node = s;
    neighbor = d;
}

PacketQueue::~PacketQueue()
{
    // Empty
}

int
PacketQueue::enq(Packet* p)
{
    if (packet_queue.size() < max_size) {
        packet_queue.push(p);
        TRACE(TRL1, "Queue size at (%d, %d) is %d\n", 
              node, neighbor, (int) packet_queue.size());
        return 1;
    }
    return 0;
}

Packet*
PacketQueue::deq()
{
    Packet* p;

    if (packet_queue.empty()) {
        return NULL;
    }
    p = packet_queue.front();
    packet_queue.pop();
    TRACE(TRL1, "Queue size at (%d, %d) is %d\n", 
          node, neighbor, (int) packet_queue.size());
    return p;
}

FIFONode::FIFONode(Address a,
                   int q) : Node(a)
{
    max_queue_size = q;
}

FIFONode::~FIFONode()
{
    // Empty
}

int
FIFONode::send(Packet* packet)
{
    Address 	 	nhop;
    PacketQueue* 	queue;
    QueueMapIterator	qiter;

    // Compute the nexthop
    nhop = topology->nexthop(address(), packet->destination);

    // Find the queue
    qiter = queue_map.find(nhop);
    if (qiter == queue_map.end()) { // An entry doesn't exist
        queue = new PacketQueue(max_queue_size, address(), nhop);
        QueueMapPair np(nhop, queue);
        queue_map.insert(np);
    } else {
        queue = (*qiter).second;
    }
    
    // Check if there is space
    if (queue->enq(packet)) {
        TRACE(TRL2, "Enqueued at %d packet (src %d, dst %d, id %d)\n",
              address(), packet->source, packet->destination, packet->id);
        send_it(nhop);
        return 1;
    }

    TRACE(TRL2, "Queue full at %d, dropped packet (src %d, dst %d, id %d)\n",
          address(), packet->source, packet->destination, packet->id);
    delete packet;
    return 0;
}

void
FIFONode::send_it(Address nhop)
{
    PacketQueue*	queue;
    QueueMapIterator	qiter;
    Packet*		pkt;

    qiter = queue_map.find(nhop);
    ASSERT(qiter != queue_map.end());
    queue = (*qiter).second;

    if (queue->pending_send) {
        return;
    }

    pkt = queue->deq();
    if (pkt) {
        TRACE(TRL2, "Forwarding at %d to nexthop %d packet (src %d, dst %d, id %d)\n",
              address(), nhop, pkt->source, pkt->destination, pkt->id);
        Node::send(pkt, nhop);
        queue->pending_send = 1;
    }
    return;
}

void
FIFONode::send_done(Address nhop)
{
    PacketQueue*	queue;
    QueueMapIterator	qiter;

    qiter = queue_map.find(nhop);
    ASSERT(qiter != queue_map.end());
    queue = (*qiter).second;

    TRACE(TRL2, "Ongoing transmission at %d to nexthop %d completed\n",
          address(), nhop);
    queue->pending_send = 0;
    send_it(nhop);
    return;
}

void
FIFONode::receive(Packet* pkt)
{
    PacketQueue*	queue;
    QueueMapIterator	qiter;
    Address		nhop;

    // If the packet is for us, drop it
    if (pkt->destination == address()) {
        delete pkt;
        return;
    }

    // Otherwise, forward packet onwards
    nhop = topology->nexthop(address(), pkt->destination);

    // Find the queue
    qiter = queue_map.find(nhop);
    if (qiter == queue_map.end()) { // An entry doesn't exist
        queue = new PacketQueue(max_queue_size, address(), nhop);
        QueueMapPair np(nhop, queue);
        queue_map.insert(np);
    } else {
        queue = (*qiter).second;
    }
    
    // Check if there is space
    if (queue->enq(pkt)) {
        TRACE(TRL2, "Forwarding at %d nexthop %d packet (src %d, dst %d, id %d)\n",
              address(), nhop, pkt->source, pkt->destination, pkt->id);
        send_it(nhop);
        return;
    }

    TRACE(TRL2, "Queue full at %d, dropped packet (src %d, dst %d, id %d)\n",
          address(), pkt->source, pkt->destination,
          pkt->id);
    delete pkt;
    return;
}

void
FIFONode::handle_timer(void* cookie)
{
    // Empty
}

