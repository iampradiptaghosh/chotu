#include "common.h"
#include "Timer.h"
#include "Packet.h"
#include "PacketScheduler.h"
#include "Scheduler.h"
#include "Topology.h"
#include "Node.h"

Node::Node(Address a)
{
    addr = a;
    scheduler->register_to_receive(this, a);
}

Node::~Node()
{
    // Empty
}

void
Node::send(Packet* packet,
     Address nhop)
{
    scheduler->send_packet(packet, nhop, this);
}

Address
Node::address() 
{
    return addr;
}

void
Node::set_timer(Time abs,
                void* cookie)
{
    scheduler->set_timer(abs,
                         cookie,
                         this);
}

void
Node::cancel_timer(Time abs,
                   void* cookie)
{
    scheduler->cancel_timer(abs,
                            cookie,
                            this);
}

void
Node::send_done(Address nhop)
{
    // Defined in derived class
}

void
Node::handle_timer(void* cookie)
{
    // Defined in derived class
}

void
Node::receive(Packet* packet)
{
    // Defined in derived class
}
