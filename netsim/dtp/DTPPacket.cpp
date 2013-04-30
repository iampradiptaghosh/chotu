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
        if(length>HEADER_SIZE)
        {
                //cout<<length-HEADER_SIZE<<endl;
                Packet::print_payload((char *) &data[0], length-HEADER_SIZE, false);
        }
}
void
DTPPacket::print_header()
{
	TRACE(TRL3, "source: %d, destination: %d, length: %d, sn: %d (%d)\n",(int) source, (int) destination, length, id, scheduler->time());
	//TRACE(TRL3, "source: %d, destination: %d, length: %d, sn: %d (%d)\nack_n: %d, Sync_bit: %d, FIN Flag: %d, ACK Flag:%d, ECN (Set by Router)= %d, ECN (Set by Host)= %d\n",(int) source, (int) destination, length, id, scheduler->time(),ack_id,sync_bit,FIN,ACK,ECN,ECN1 );
}
