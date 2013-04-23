#include "../netsim/common.h"
#include "../netsim/Node.h"
#include "../netsim/FIFONode.h"
#include "../netsim/Packet.h"
#include "DTPPacket.h"
#include "../netsim/Timer.h"
#include "../netsim/PacketScheduler.h"
#include "../netsim/Scheduler.h"
// DTPPacket::DTPPacket()
// {
	// sync_bit=0;
// }
void
DTPPacket::print()
{
    print_header();
    //if(length>HEADER_SIZE+sizeof(Packet))
    //Packet::print_payload((char *) &data[0], length-(HEADER_SIZE+sizeof(Packet)), false);
}
void
DTPPacket::print_header()
{
	TRACE(TRL3, "Packet Header:: source: %d, destination: %d, id: %d, ack_id %d, length: %d, Sync_bit: %d, FIN Flag: %d, ACK Flag:%d, ECN (Set by Router)= %d, ECN (Set by receiver)= %d, Remaining File_Size = %ld bytes\n",
          (int) source, (int) destination, id, ack_id, length,sync_bit,FIN,ACK,ECN,ECN1,file_size );
}
