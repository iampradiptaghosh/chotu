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
#define timeout 4000
Host::Host(Address a) : FIFONode(a,MAX_QUEUE)	// Null queue size
{
   sync_bit=1;
   term_bit=0;
   retrans=0;
   retrans_bit=0;
   packets_in_window=0;
   window_size=5;
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
	write=0;
	if(((DTPPacket*)pkt)->id==recv_so_far+1)
	{
	        recv_so_far+=1;
	        write=1;
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
				retrans=0;
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
		else if(((DTPPacket*)pkt)->term_connection==0)
		{
		
		
		if(!sender)
		{
		        if(sync_bit==2)
		        {
		                sync_bit=3;
		                if(retrans>=scheduler->time())
			        {
			        	cancel_timer(retrans, NULL);
				        retrans_bit=0;
			        	retrans=0;
			        }
			 }
			 recv_window_sync((DTPPacket*)pkt);
		}
		else
		{
		             
	                //cout<<"jddjdkdkd"<<endl;
	                sent_window_sync((DTPPacket*)pkt);
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
			
			//out_file.close();
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
    
	DTPPacket*	pkt = new DTPPacket;
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
        if(sync_bit==3&&term_bit==0)
	{
		if(retrans_bit==2)
		{	
			cout<<"kk";
			last_transmit=scheduler->time();
			retrans=scheduler->time() +retrans_timer;
			set_timer(retrans, NULL);
			retrans_bit=2;
			cout<<"kk";
			if (send(retransmit_pkt)) {
                                        TRACE(TRL3,"Retransmit packet from DTP-Host: %d\n",address());
	                retransmit_pkt->print_header();
	                }
	                cout<<"kk";
	                goto timer_end;
		}
	}
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
	
	timer_end: return;
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
        strcpy((((Host*)nd)->out_file),name);
	
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
	        while (!in_file.eof())
                {
	                line[i]=in_file.get();
	                i++;
	                if(i==PAYLOAD_SIZE-1)
	                        break;
	        }
	        if(in_file.eof())
	                i-=1;
	        line[i]='\0';     
                DTPPacket* pkt= new DTPPacket;
                DTPPacket* pkt11= new DTPPacket;
                char* d;

                sent_so_far+=1;
                d = &(pkt->data[0]);
                strcpy(d,line);
	        //cout<<d<<endl;
	        pkt->term_connection=term_bit;
                pkt->source = address();
                pkt->destination = destination;
                pkt->length = sizeof(Packet)+HEADER_SIZE+PAYLOAD_SIZE;
	        //fprintf(stderr,"\n %d",sent_so_far)
	        pkt->id = sent_so_far;
	        pkt->ack_id =recv_so_far;
 	        pkt->sync_bit=sync_bit;
 	        copy_pkt(pkt11,pkt);
 	        if (send(pkt)) 
                {
                        TRACE(TRL3,"Sent packet from DTP-Host: %d\n",address());
	                pkt->print_header();
	        }
	        {
	                /*int sss=(pkt11->id);
	                cout<<sss<<endl;
	                Window1_iter head2;
	                DTPPacket* pkt1;
	                if(!sent_window.empty())
	                {
	                head2 =sent_window.begin();
	                pkt1 = (*head2).second;
	                 cout<<"aaaa"<<endl<<endl;
	                pkt1->print_header();
	                }*/
	                Window1Pair entry(pkt11->id,pkt11);
	                sent_window.insert(entry);  
	                /*head2 =sent_window.begin();
	                pkt1 = (*head2).second;
	                 cout<<"aaaa"<<endl<<endl;
	                pkt1->print_header();*/
	        } 
        }
        in_file.close();
        }
        terminate(destination);
        ss:     Window1_iter head1 =sent_window.begin();
	        if(!sent_window.empty())
	        {
	                retransmit_pkt=(*head1).second;
	                cout<<"fjfjfjfjf"<<endl<<endl;
	                retransmit_pkt->print_header();
	        }
	        else
                        retransmit_pkt=NULL;
        if(retrans_bit==0&&retransmit_pkt!=NULL)
	{
	        retrans_bit=2;
	       	retrans=scheduler->time() +retrans_timer;
	       	//set_timer(retrans, NULL);
	}
	//display();		        
        return;
}

void Host:: recv_window_sync(DTPPacket* pkt)
{
      	DTPPacket* pkt11=new DTPPacket;;
        copy_pkt(pkt11,pkt);//cout<<"aaaaaa"<<endl;
      	Window1_iter head = recv_window.find(pkt11->id);
      	ofstream file1(out_file, ios::out | ios::app);
	if(head!=recv_window.end())
	{
	        //handle_timer((void*)1);
	}
	else if((pkt11->id)<recv_so_far)
	{
	        //handle_timer((void*)1);
	}
	else if((pkt11->id)==recv_so_far&&write)
	{
	       
	        cout<<"aa:"<<pkt11->data;
	        if (file1.is_open()&&file1.good())
	        {
	               // cout<<"hh"<<endl;
	                file1<<pkt11->data;
	        }
                DTPPacket *pkt1;
	        Window1_iter head1 =recv_window.begin();
	        while(head1!=recv_window.end())
	        {
	                pkt1=(*head1).second;
	                if(pkt1->id!=recv_so_far+1)
	                {
	                        break;
	                }
	                recv_so_far+=1;
	                file1<<pkt1->data;
	                //cout<<"bb"<<pkt1->data<<endl;
	                recv_window.erase(head1);
	                head1 =recv_window.begin();
	        }
	                
         }
         else
         {      Window1Pair entry(pkt11->id,pkt11);
	        recv_window.insert(entry);
	        //set_timer(retrans, NULL);
	 }      
	// display(recv_window);
	 file1.close();
	 handle_timer((void*)1);
}

void Host:: sent_window_sync(DTPPacket* pkt)
{
        //cout<<address()<<endl;
        //display();
        //cout<<"jddjdkdkd"<<endl;
        if(pkt->ack_id<=last_ack)
        {
                //retransmit();
                return;
        }
        //cout<<"jddjdkdkd"<<endl;
        if(retrans>=scheduler->time())
	{
	        if(last_ack<pkt->ack_id)
	        {
	                  // cancel_timer(retrans, NULL);
	                   retrans=0;
	                   retrans_bit=0;
	                   //scheduler->time() +retrans_timer;
	                   //set_timer(retrans, NULL);
	        }
	}
	cout<<"cccc"<<endl<<endl;
	pkt->print_header();
        DTPPacket *pkt1;
        Window1_iter head = sent_window.find(pkt->ack_id);
	if(head!=sent_window.end()&&!sent_window.empty())
	{
	        //cout<<"jddjdkdkd"<<endl;
	        Window1_iter head1 =sent_window.begin();
	        if(head1==head)
	        {
	                pkt1=(*head1).second;
	                cout<<"bbbb"<<endl<<endl;
	                pkt1->print_header();
	                last_ack=pkt1->ack_id;
	                sent_window.erase(head1);
	                packets_in_window--;
	        }
	        else
	        {
	        while(head1!=head)
	        {
	                head1 =sent_window.begin();
	                pkt1=(*head1).second;
	                cout<<"bbbb"<<endl<<endl;
	                pkt1->print_header();
	                last_ack=pkt1->ack_id;
	                sent_window.erase(head1);
	                packets_in_window--;
	        }
	        }
	        //update window
	        //retransmit
	}
	
	//display();
	
	
	Window1_iter head1 =sent_window.begin();
	if(head1!=sent_window.end())
	        retransmit_pkt=(*head1).second;
	else
                retransmit_pkt=NULL;
                
        if(retrans_bit==0&&retransmit_pkt!=NULL)
        {
                retrans_bit=2;
                retrans=scheduler->time() +retrans_timer;
                //set_timer(retrans, NULL);
        }       
        
        return;
}
void Host::display(Window1 w1)
{
        
        
        ;if(!w1.empty())
	{
	        //cout<<"jddjdkdkd"<<endl;
	        Window1_iter head1 =w1.begin();
	        DTPPacket* pkt1;
	        while(head1!=w1.end())
	        {
	                pkt1=(*head1).second;
	                cout<<"dddd"<<endl<<endl;
	                pkt1->print_header();
	                head1=w1.upper_bound(pkt1->id);
	        }
	}
}
void Host::copy_pkt(DTPPacket* pkt_to,DTPPacket* pkt_from)
{
                pkt_to->term_connection=pkt_from->term_connection;
                pkt_to->source = pkt_from->source;
                pkt_to->destination = pkt_from->destination;
                pkt_to->length = pkt_from->length;
	        pkt_to->id = pkt_from->id;
	        pkt_to->ack_id =pkt_to->ack_id;
 	        pkt_to->sync_bit=pkt_from->sync_bit; 
                strcpy(pkt_to->data,pkt_from->data);
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
