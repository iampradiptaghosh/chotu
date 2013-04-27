#include "../netsim/common.h"
#include "../netsim/Node.h"
#include "../netsim/FIFONode.h"
#include "../netsim/PacketScheduler.h"
#include "../netsim/Timer.h"
#include "../netsim/Scheduler.h"
#include "../netsim/Topology.h"
#include "Router.h"
#include "FDTPFlow.h"
#include "Host.h"
#include "../netsim/Packet.h"
#include "DTPPacket.h"


PacketQueue1::PacketQueue1(int sz,
                         Address s,
                         Address d)
{
    pending_send = 0;
    max_size = sz;
    node = s;
    neighbor = d;
}

PacketQueue1::~PacketQueue1()
{
    // Empty
}

int
PacketQueue1::enq(Packet* p)
{
    
    if (packet_queue.size() < max_size) {
        if (packet_queue.size() >= max_size/3)
        {
                //cout<<"Set ECN"<<endl;
                ((DTPPacket*)p)->ECN=1;
                //cout<<((DTPPacket*)p)->ECN<<endl;
        }
        packet_queue.push(p);
        TRACE(TRL1, "Queue size at (%d, %d) is %d\n", 
              node, neighbor, (int) packet_queue.size());
        return 1;
    }
    return 0;
}

Packet*
PacketQueue1::deq()
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

Router::Router(Address a, int b) : FIFONode(a,b)	// Null queue size
{
    max_queue_size = b;
    TRACE(TRL3, "Initialized router with address %d\n",a);// Empty
}

Router::~Router()
{
    // Empty
}


int
Router::send(Packet* packet)
{
    Address 	 	nhop;
    PacketQueue1* 	queue;
    QueueMapIterator1	qiter;

    // Compute the nexthop
    nhop = topology->nexthop(address(), packet->destination);

    // Find the queue
    qiter = queue_map.find(nhop);
    if (qiter == queue_map.end()) { // An entry doesn't exist
        queue = new PacketQueue1(max_queue_size, address(), nhop);
        QueueMapPair1 np(nhop, queue);
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
Router::send_it(Address nhop)
{
    PacketQueue1*	queue;
    QueueMapIterator1	qiter;
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
Router::send_done(Address nhop)
{
    PacketQueue1*	queue;
    QueueMapIterator1	qiter;

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
Router::receive(Packet* pkt)
{
    PacketQueue1*	queue;
    QueueMapIterator1	qiter;
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
        queue = new PacketQueue1(max_queue_size, address(), nhop);
        QueueMapPair1 np(nhop, queue);
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
    //if (pkt)
    //((DTPPacket*)pkt)->print();
    delete pkt;
    return;
}

void
Router::handle_timer(void* cookie)
{
    // Empty
}

