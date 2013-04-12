#include <cstdio>

#define PAYLOAD_SIZE (MTU - sizeof(Packet))
#define HEADER_SIZE (2*sizeof(int))
//
// DTPPacket is derived from Packet and adds a data field.
//

class DTPPacket : public Packet {
 public:
	// DTPPacket();
	// ~DTPPacket();
	int sync_bit;
    int term_connection;
	void print();
	void print_header();
        char data[PAYLOAD_SIZE];
};
