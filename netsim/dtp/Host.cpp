#include "../netsim/common.h"
#include "../netsim/Node.h"
#include "../netsim/FIFONode.h"
#include "../netsim/Packet.h"
#include "DTPPacket.h"
#include "../netsim/PacketScheduler.h"
#include "../netsim/Timer.h"
#include "../netsim/Scheduler.h"
#include "Host.h"
#include "Router.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#define MAX_QUEUE 99999
#define timeout 4000000
Host::Host(Address a) : FIFONode(a,MAX_QUEUE)	// Null queue size
{
   sync_bit=1;
   term_bit=0;
   retrans=0;
   retrans_bit=0;
   packets_in_window=0;
   window_size=2;
   retrans_timer=timeout;
   TRACE(TRL3,"Initialized host with address %d\n",a);// Empty
}

Host::~Host()
{
    // Empty
}
void
Host::get_dest(Packet* pkt)
{
	destination=((DTPPacket*)pkt)->source;
}
void
Host::receive(Packet* pkt)
{
   	TRACE(TRL3,"Received packet at DTP-Host: %d\n",address());
	((DTPPacket*)pkt)->print_header();
	if(((DTPPacket*)pkt)->id==recv_so_far+1)
	{
	        recv_so_far+=1;
	}
	
	if(term_bit==0)
	{
		if (((DTPPacket*)pkt)->sync_bit==1&&sync_bit==1)
		{
			get_dest(pkt);
			sync_bit=2;
			TRACE(TRL4,"(Time:%d) node %d Got SYN from node %d\n",scheduler->time(),address(),destination);
			handle_timer((void*)1);
			retrans=scheduler->time() +retrans_timer;
			set_timer(retrans, NULL);
			retrans_bit=1;
			TRACE(TRL4,"(Time:%d) node %d Sent SYN-ACK to node %d\n",scheduler->time(),address(),destination);
			
		}
		else if (((DTPPacket*)pkt)->sync_bit==2&&sync_bit==3)
		{
			handle_timer((void*)1);
		
		}
		else if (((DTPPacket*)pkt)->sync_bit==2&&sync_bit==1)
		{
			if(retrans>=scheduler->time())
			{
				cancel_timer(retrans, NULL);
				retrans_bit=0;
			}
			get_dest(pkt);	
			sync_bit=3;
			retrans_timer=1.5*(scheduler->time()-last_transmit);
			TRACE(TRL4,"(Time:%d) node %d Got SYN-ACK from node %d\n",scheduler->time(),address(),destination);
			
			//fprintf(stderr,"Got SYN, node %d, %d %d\n",address(),retrans_timer,last_transmit);
			//fprintf(stderr,"Got SYN-ACK, node %d\n",address());	
			handle_timer((void*)1);
			TRACE(TRL4,"(Time:%d) node %d Sent ACK to node %d\n",scheduler->time(),address(),destination);
			send_file();
			
			//terminate(destination);
			
			
		
		}
		else if (((DTPPacket*)pkt)->sync_bit==3&&sync_bit==2)
		{
			//fprintf(stderr,"Got ACK, node %d\n",address());
			if(retrans>=scheduler->time())
			{
				cancel_timer(retrans, NULL);
				retrans_bit=0;
			}
			if(sync_bit!=3)
			{
				sync_bit=3;
				get_dest(pkt);
				Time tm=scheduler->time();
				TRACE(TRL4,"(Time:%d) node %d Got ACK from node %d\n",scheduler->time(),address(),destination);
			        
				TRACE(TRL3, "Established FDTP flow from %d to %d (%d)\n",  destination,address(),tm);
				//out_file.open("example.txt", ios::out);
				//term_bit=1;
				
			/* 	Node* nd = (scheduler->get_node)(destination);
				((Host*)nd)->terminate(address()); */
			}
		}
		else
		{
		
		
		if(!sender)
		{
		        out_file<<(((DTPPacket*)pkt)->data);
		        handle_timer((void*)1);
		}
		else
		{
		             
	                packets_in_window--;
		        send_file();
		}
		/* 		if(retrans>=scheduler->time())
		{
				cancel_timer(retrans, NULL);
				retrans_bit=0;
		} */
		//fprintf(stderr,"Dropped, node %d\n",address());
		}
	
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	if(sync_bit==3||sync_bit==2)
	{
		if (((DTPPacket*)pkt)->term_connection==1&&term_bit==3&&sync_bit==3)
		{
			TRACE(TRL4,"(Time:%d) node %d Got Duplicate FIN from node %d\n",scheduler->time(),address(),destination);
			//fprintf(stderr,"Got FIN, node %d\n",address());
			get_dest(pkt);
			term_bit=2;
			handle_timer((void*)1);
			term_bit=3;
			TRACE(TRL4,"(Time:%d) node %d Sent Duplicate FIN-ACK to node %d\n",scheduler->time(),address(),destination);
			
		}
		else if (((DTPPacket*)pkt)->term_connection==1&&term_bit!=3)//&&term_bit==1)
		{
			
			out_file.close();
			{ 
			if(sync_bit==2)
				TRACE(TRL3, "Established FDTP flow from %d to %d (%d)\n",  destination,address(),scheduler->time());
			}
			TRACE(TRL4,"(Time:%d) node %d Got FIN from node %d\n",scheduler->time(),address(),destination);
			sync_bit=3;
			//fprintf(stderr,"Got FIN, node %d\n",address());
			get_dest(pkt);
			term_bit=2;
			handle_timer((void*)1);
			TRACE(TRL4,"(Time:%d) node %d Sent FIN-ACK to node %d\n",scheduler->time(),address(),destination);
			term_bit=3;
			handle_timer((void*)1);
			retrans=scheduler->time() +retrans_timer;
			set_timer(retrans, NULL);
			retrans_bit=1;
			TRACE(TRL4,"(Time:%d) node %d Sent FIN to node %d\n",scheduler->time(),address(),destination);
			
		}
		
		else if (((DTPPacket*)pkt)->term_connection==2&&term_bit==1&&sync_bit==3)
		{
			if(retrans>=scheduler->time())
			{
				cancel_timer(retrans, NULL);
				retrans_bit=0;
			}
			//get_dest(pkt);
			term_bit=3;
			TRACE(TRL4,"(Time:%d) node %d Got FIN-ACK from node %d\n",scheduler->time(),address(),destination);
			
			//fprintf(stderr,"Got FIN-ack, node %d (%d)\n",address(),scheduler->time());
			//handle_timer((void*)1);
		}
		else if (((DTPPacket*)pkt)->term_connection==3&&term_bit==4&&sync_bit==3)
		{
			//term_bit=4;
			TRACE(TRL4,"(Time:%d) node %d Got Duplicate FIN from node %d\n",scheduler->time(),address(),destination);
			handle_timer((void*)1);
			TRACE(TRL4,"(Time:%d) node %d Sent Duplicate FIN-ACK to node %d\n",scheduler->time(),address(),destination);
			//handle_timer((void*)1);
		}
		else if (((DTPPacket*)pkt)->term_connection==3&&term_bit==3&&sync_bit==3)
		{
			TRACE(TRL4,"(Time:%d) node %d Got FIN from node %d\n",scheduler->time(),address(),destination);
			term_bit=4;
			handle_timer((void*)1);
			TRACE(TRL4,"(Time:%d) node %d Sent FIN-ACK to node %d\n",scheduler->time(),address(),destination);
		}
		
		else if (((DTPPacket*)pkt)->term_connection==4&&term_bit==3&&sync_bit==3)
		{
			if(retrans>=scheduler->time())
			{
				cancel_timer(retrans, NULL);
				retrans_bit=0;
			}
			get_dest(pkt);	
			Time tm=scheduler->time();
			TRACE(TRL4,"(Time:%d) node %d Got FIN-ACK from node %d\n",scheduler->time(),address(),destination);
			
			TRACE(TRL3, "Tore down FDTP flow from %d to %d (%d)\n", destination, address(),tm);
			sync_bit=1;
			term_bit=0;
			retrans=0;
			retrans_bit=0;
			Node* nd = (scheduler->get_node)(destination);
			((Host*)nd)->sync_bit=1;
			((Host*)nd)->term_bit=0;
			((Host*)nd)->retrans=0;
			((Host*)nd)->retrans_bit=0;
		}
		else
		{
		/* if(retrans>=scheduler->time())
		{
				cancel_timer(retrans, NULL);
			window_size	retrans_bit=0;
		} */
			//fprintf(stderr,"Dropped, node %d\n",address());
		}
	}
	
    delete pkt;
} 
void
Host::handle_timer(void* cookie)
{
    
	if(sync_bit==1&&term_bit==0)
	{
		if(retrans==0)
		{	sync();
			last_transmit=scheduler->time();
			retrans=scheduler->time() +retrans_timer;
			set_timer(retrans, NULL);
			retrans_bit=1;
			TRACE(TRL4,"(Time:%d) node %d Sent SYN to node %d\n",scheduler->time(),address(),destination);
		}
	}
    DTPPacket*	pkt = new DTPPacket;
    //char* d = &(pkt->data[0]);
    pkt->term_connection=term_bit;
    pkt->source = address();
    pkt->destination = destination;
    pkt->length = sizeof(Packet)+HEADER_SIZE;
    sent_so_far+=1;
	if(retrans==scheduler->time()&&retrans_bit==1)
	{
		 sent_so_far-=1;
//fprintf(stderr,"Node=%d retrans= %d, Time= %d\n",address(),retrans,scheduler->time());
		retrans=scheduler->time() +retrans_timer;
		set_timer(retrans, NULL);
		TRACE(TRL3,"RETRANSMIT");
//fprintf(stderr,"Node=%d retrans= %d, Time= %d\n",address(),retrans,scheduler->time());
	} 
	//fprintf(stderr,"\n %d",sent_so_far)
	pkt->id = sent_so_far;
	pkt->ack_id =recv_so_far;
 	pkt->sync_bit=sync_bit;
    if (send(pkt)) {
       TRACE(TRL3,"Sent packet from DTP-Host: %d\n",address());
	   pkt->print_header();
	   }
	
	return;
}
void
Host::sync()
{
	SendMap_iter head = dest_map.find(scheduler->time());
	sendpair* newpair=(*head).second;
	destination = newpair->a;
        dest_map.erase(head);
	packets_to_send = 1;
	cout<<newpair->name<<endl;
	in_file.open (newpair->name, ios::in); 
	Node* nd = (scheduler->get_node)(destination);
	char name[99999];
	strcpy(name,newpair->name);
	strcat(name,"_dest");
        (((Host*)nd)->out_file).open (name, ios::out);
	
    //sent_so_far = 0;
}
void
Host::send_file()
{
        char line[PAYLOAD_SIZE];
        char c;
        int i=0;
	if (in_file.is_open())
         {
               
                while ( in_file.good()&&!in_file.eof())
        {
        if(packets_in_window>=window_size)
	   {    
	        goto ss;
	   }
	   else
	   {
	        packets_in_window++;
	   }
	   i=0;
	   while ( in_file.good()&&!in_file.eof())
           {
	        line[i]=in_file.get();
	        i++;
	        if(i==PAYLOAD_SIZE-1)
	                break;
	   }
	   if(in_file.eof())
	   i--;
	   line[i]='\0';     
        DTPPacket*	pkt = new DTPPacket;
        char* d = &(pkt->data[0]);
        strcpy(d,line);
	cout<<d<<endl;
	pkt->term_connection=term_bit;
        pkt->source = address();
        pkt->destination = destination;
        pkt->length = sizeof(Packet)+HEADER_SIZE+PAYLOAD_SIZE;
        sent_so_far+=1;
	//fprintf(stderr,"\n %d",sent_so_far)
	pkt->id = sent_so_far;
	pkt->ack_id =recv_so_far;
 	pkt->sync_bit=sync_bit;
        cout << line << endl;
         if (send(pkt)) {
       TRACE(TRL3,"Sent packet from DTP-Host: %d\n",address());
	   pkt->print_header();
	   }
	   
	   
        }
        in_file.close();
        }
        terminate(destination);
   ss:     return;
}

void Host::insert_p(Time s,Address d,char* f)
{
	sendpair* newpair=new sendpair;
	newpair->a=d;
	strcpy(newpair->name,f);
	SendMapPair entry(s,newpair);
	dest_map.insert(entry);
}
void
Host::terminate(Address d)
{
	DTPPacket*	pkt = new DTPPacket;
    destination = d;
	term_bit=1;
	handle_timer((void*)1);
	retrans=scheduler->time() +timeout;
    set_timer(retrans, NULL);
	retrans_bit=1;
	TRACE(TRL4,"(Time:%d) node %d Sent FIN to node %d\n",scheduler->time(),address(),destination);
			
}
