class Packet;
class Node;

//
// As its name suggests, PacketQueue maintains a FIFO queue of
// packets. At each FIFONode, there is one such queue for every
// nexthop. The queue size is configurable.
//

class PacketQueue {
    friend class FIFONode;
    
 public:
    PacketQueue(int sz, Address s, Address d);
    ~PacketQueue();

    // Return 0 if no space in queue, else 1
    int enq(Packet* p);			// Pointer to packet to send

    // Return first packet, else NULL
    Packet* deq();

 private:
    queue<Packet*>	packet_queue;	// Queue of packets
    int			max_size;	// Max queue size
    int 		pending_send;	// Set if packet is in transit
    Address		node;		// Node at which queue resides
    Address		neighbor;	// Neighbor for queue
};

typedef map<Address, PacketQueue*, ltAddress> QueueMap;
typedef map<Address, PacketQueue*, ltAddress>::iterator QueueMapIterator;
typedef pair<Address, PacketQueue*> QueueMapPair;

//
// The FIFONode implements a FCFS router. Most routers in the Internet
// employ this queuing discipline. For each neighbor, it maintains a
// PacketQueue.
//
// The CbrSender class is derived from FIFONode. 
//

class FIFONode : public Node {
    
 public:
    FIFONode(Address a, 			// Node address
             int queue_size);			// Number of packets
    ~FIFONode();

    // Send a packet: return 0 on failure, 1 on success
    int send(Packet* packet);		// packet to send
    
    // Call when the packet has been sent
    void send_done(Address nhop);	// Neighbor to whom sent
    
    // Receive a packet
    // Needs to be defined by the inheriting class
    void receive(Packet* packet);	// received packet

    // Handle a timer
    // Needs to be defined by the inheriting class
    void handle_timer(void* cookie);

 private:
    // Send a packet to a nexthop
    void send_it(Address nhop);
    
    int		max_queue_size;			// Number of packets in queue
    QueueMap	queue_map;			// Map of queues per nexthop
    int		pending_send;			// Boolean
};

