class Packet;

//
// The Node class represents a generic node. This class provides the
// glue between FIFONode and the discrete event simulator. As such,
// its functionality is simple, and many of its functions are virtual.
//
// FIFONode is derived from Node.
//

class Node {
    
public:
    Node(Address a);			// Node address
    ~Node();

    // Set an absolute timer
    void
    set_timer(Time abs,		// absolute interval 
              void* cookie);	// returned when timer fires

    // Cancel a previously specified timer
    void
    cancel_timer(Time abs,	// absolute interval 
                 void* cookie);	// returned when timer fires

    // Send a packet
    void send(Packet* packet,		// packet to send
              Address nhop);		// Next hop address
    
    // Call when the packet has been sent
    virtual void send_done(Address nhop);	// Neighbor to whom sent
    
    // Receive a packet
    // Needs to be defined by the inheriting class
    virtual void receive(Packet* packet);

    // Handle a timer
    // Needs to be defined by the inheriting class
    virtual void handle_timer(void* cookie);

    // Return the node address
    Address address();

private:
    Address	addr;

};
