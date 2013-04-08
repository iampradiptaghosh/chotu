#include "common.h"
#include "Packet.h"
#include "Node.h"
#include "PacketScheduler.h"
#include "Timer.h"
#include "Scheduler.h"
#include "Topology.h"

// Globally visible topology instance
Topology* topology;

LinkData::LinkData(Address n,
                   Time p,
                   Time b)
{
    neighbor = n;
    plat = p;
    bit_time = b;
}

LinkData::~LinkData()
{
    // Empty
}

Topology::Topology()
{
    // Empty
}

Topology::~Topology()
{
    linkMap.clear();
}

// Find the specified link in the multimap, returns NULL if none
LinkData*
Topology::find_link(Address s,
                    Address d)
{
    pair<LinkMapIterator, LinkMapIterator> it_pair = linkMap.equal_range(s);

    for (LinkMapIterator it = it_pair.first; it != it_pair.second; ++it) {
        LinkData* link = (*it).second;
        if (link->neighbor == d) {
            return link;
        }
    }
    return NULL;
}

// Erase the entry for the corresponding link, in the specified direction
void
Topology::erase_link(Address s,
                     Address d)
{
    pair<LinkMapIterator, LinkMapIterator> it_pair = linkMap.equal_range(s);

    for (LinkMapIterator it = it_pair.first; it != it_pair.second; ++it) {
        LinkData* link = (*it).second;
        if (link->neighbor == d) {
            linkMap.erase(it);
            return;
        }
    }
}

// Add a link to the link map
// If duplicate link entries are specified, the last one is used
void
Topology::add_link(Address s,
                   Address d,
                   Time p,
                   Time b)
{
    // Remove duplicates
    if (find_link(s, d)) {
        erase_link(s,d);
    }
    if (find_link(d,s)) {
        erase_link(d,s);
    }
        
    // Insert the link
    LinkData*	link1 = new LinkData(d, p, b);
    LinkData* 	link2 = new LinkData(s, p, b);

    linkMap.insert(LinkMapPair(s, link1));
    linkMap.insert(LinkMapPair(d, link2));

    TRACE(TRL1,
          "Added link between %d and %d, prop. latency %d, bit time %d\n",
          s, d, p, b);
}

// Compute the total packet delay on specified link
Time
Topology::link_delay(Packet* p,
                     Address s,
                     Address d)
{
    LinkData* ld = find_link(s,d);

    ASSERT(ld != NULL);

    return (ld->plat + ((p->length * 8) * ld->bit_time));
}

// Compute the total packet delay on specified link
Time
Topology::link_trans_delay(Packet* p,
                           Address s,
                           Address d)
{
    LinkData* ld = find_link(s,d);

    ASSERT(ld != NULL);

    return ((p->length * 8) * ld->bit_time);
}

// Do a BFS computation to find the nexthop
Address
Topology::nexthop(Address s, 	// Node at which packet currently is
                  Address d)	// Destination of packet
{
    queue<Address>	bfsq;
    Address		current;

    // Start with the destination and work backward
    // Assumes symmetric paths, which is fine for now
    bfsq.push(d);

    while (!bfsq.empty()) {
        current = bfsq.front();
        bfsq.pop();

        pair<LinkMapIterator, LinkMapIterator> it_pair = 
            linkMap.equal_range(current);	// all neighbors of "current"

        for (LinkMapIterator it = it_pair.first; it != it_pair.second; ++it) {
            LinkData* link = (*it).second;
            if (link->neighbor == s) {
                TRACE(TRL1, 
                      "Nexthop from %d towards %d is %d\n", 
                      s, d, current);
                return current;
            } else {
                bfsq.push(link->neighbor);
            }
        }
    }

    // We can't come here
    ASSERT(false);
}
