#include <cstdio>

#define MTU 1500		// ... in bytes the total size of any packet

//
// The Packet class describes the actual data that is transmitted
// between nodes. It has several fields that are analogous to fields
// in the IP header. Data packets can be derived from this base class
// (see the CBR app for an example)
//

class Packet {
 public:
    virtual ~Packet(void) {};
    void print_header();
    void print_payload(char* p,			// pointer to payload
                       unsigned int len,	// length of payload
                       bool ascii);		// hex if false, ascii if true

    Address 		source;		// Sender node address
    Address		destination;	// Destination node address
    unsigned int	length;		// Total length, in bytes
    unsigned int	id;		// Unused by netsim
};
