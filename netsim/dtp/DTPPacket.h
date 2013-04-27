#include <cstdio>


#define HEADER_SIZE (sizeof(Packet)+(sizeof(int)+sizeof(short)+4*sizeof(bool)))
#define PAYLOAD_SIZE ((MTU - sizeof(Packet)-HEADER_SIZE)-5)
//
// DTPPacket is derived from Packet and adds a data field.
//

class DTPPacket : public Packet {
 public:
	// DTPPacket();
        virtual ~DTPPacket(void) {};
	short sync_bit;
        bool FIN;
        bool ECN;
        bool ECN1;
        bool ACK;
        int ack_id;
	void print();
	void print_header();
        unsigned char data[PAYLOAD_SIZE];
};
