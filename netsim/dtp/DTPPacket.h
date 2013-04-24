#include <cstdio>


#define HEADER_SIZE sizeof(Packet)+(sizeof(int)+sizeof(short)+4*sizeof(bool))
#define PAYLOAD_SIZE (MTU - sizeof(Packet)-HEADER_SIZE)
//
// DTPPacket is derived from Packet and adds a data field.
//

class DTPPacket : public Packet {
 public:
	// DTPPacket();
	// ~DTPPacket();
	short sync_bit;
        bool FIN;
        bool ECN;
        bool ECN1;
        bool ACK;
        int ack_id;
	void print();
	void print_header();
        char data[PAYLOAD_SIZE];
};
