
//
// LinkData maintains the information about a link; its propagation
// latency, its transmission latency, and the identity of the remote
// neighbor.
//

class LinkData {

    friend class Topology;
    
 public:
    LinkData(Address a, Time p, Time b);
    ~LinkData();
    
 private:
    Address		neighbor;	// Address of neighbor
    Time		plat;		// Propagation latency
    Time		bit_time;	// Time taken to transmit one bit
};

//
// LinkData structures are maintained in a LinkMap. A LinkMap is
// essentially an adjacency list: for each node in the network,
// LinkMap maintains a list of links attached to that node.
// This is implemented as an STL multi-map.
//

typedef multimap<Address, LinkData*, ltAddress> LinkMap;
typedef multimap<Address, LinkData*, ltAddress>::iterator LinkMapIterator;
typedef pair<Address, LinkData*> LinkMapPair;

//
// The Topology uses the LinkMap, obtained from the configuration
// file, and essentially implements shortest path routing. It's main
// function, nexthop(), finds the next hop from a given node to a
// specified destination. It also computes the time that it would take
// to transmit a packet to a given nexthop.
//

class Topology {
    friend class Scheduler;

public:
    Topology();
    ~Topology();		

    // Return the transmission and propagation delay on link
    Time link_delay(Packet* p,		// Packet to be sent
                    Address s,		// Sender
                    Address d);		// Target

    // Return the transmission delay on link
    Time link_trans_delay(Packet* p,		// Packet to be sent
                          Address s,		// Sender
                          Address d);		// Target


    // Return the next hop for a given node
    Address nexthop(Address s,		// Sender
                    Address d);		// Target

    // Add a link to the topology
    void add_link(Address a,		// One end of link
                  Address b,		// Other end of link
                  Time p,		// Propagation delay in ticks
                  Time tb);		// Time per bit on link

private:
    // Return the iterator pointing to the link
    LinkData* find_link(Address s,
                        Address d);

    // Erase link from topology
    void erase_link(Address s,
                    Address d);
    
    LinkMap		linkMap;
};

extern Topology* topology;
