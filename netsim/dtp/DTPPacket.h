#include <cstdio>

#define PAYLOAD_SIZE 13//(MTU - sizeof(Packet))
#define HEADER_SIZE (2*sizeof(int))
//
// DTPPacket is derived from Packet and adds a data field.
//

class DTPPacket : public Packet {
 public:
	// DTPPacket();
	// ~DTPPacket();
	short sync_bit;
        bool FIN;
        bool ACK;
        int ack_id;
	void print();
	void print_header();
        char data[PAYLOAD_SIZE];
};
