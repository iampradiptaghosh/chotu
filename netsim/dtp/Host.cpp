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
#include <math.h>
#include <stdio.h>
using namespace std;
#define MAX_QUEUE 99999
#define timeout 4000
#define K 4
#define G 0.5
#define alpha 0.125
#define beta 0.25
#define Initial_Window 1
#define Initial_Window_Threshold 10000
Host::Host(Address a) : FIFONode(a,MAX_QUEUE)	// Null queue size
{
        sync_bit=1;     term_bit=0;     retrans=0;      retrans_bit=0;
        
        window_size=Initial_Window;
        
        Window_threshold=Initial_Window_Threshold;
                
        RTO=timeout;    RTT=0;  RTTVAR=0;       ECN=0;  flag=0;
        
        last_ack=0;     retransmit_pkt=NULL;
        
        sent_window.clear();    recv_window.clear();    send_time.clear();
        
        sender=0;       alpha1=0;       ACKcount_flag=0;
        
        dup_ack_count=0;        ECNcount=0;     ACKcount=0;     ACKcounter=0;
        
        back_off_counter=1;     last_transmit=0;        ter_seq=MAX_QUEUE;
        
        sent_so_far=0;  recv_so_far=0;  packets_in_window=0;    
        
        TRACE(TRL3,"Initialized host with address %d\n",a);
}

Host::~Host()
{
    // Empty
}
void
Host::reset()
{
        sync_bit=1;     term_bit=0;     retrans=0;      retrans_bit=0;
        
        window_size=Initial_Window;
        
        Window_threshold=Initial_Window_Threshold;
                
        RTO=timeout;    RTT=0;  RTTVAR=0;       ECN=0;  flag=0;
        
        last_ack=0;     retransmit_pkt=NULL;
        
        sent_window.clear();    recv_window.clear();    send_time.clear();
        
        sender=0;       alpha1=0;       ACKcount_flag=0;
        
        dup_ack_count=0;        ECNcount=0;     ACKcount=0;     ACKcounter=0;
        
        back_off_counter=1;     last_transmit=0;        ter_seq=MAX_QUEUE;
        
        sent_so_far=0;  recv_so_far=0;  packets_in_window=0;    
        
}
void
Host::get_dest(Packet* pkt)
{
	destination=((DTPPacket*)pkt)->source;
}
void
Host::receive(Packet* pkt)
{
   	if(sync_bit!=3)
   	{
   	        TRACE(TRL3,"Received packet at DTP-Host: %d\n",address());
	        ((DTPPacket*)pkt)->print();
	}
	else
   	{
   	        if(sender)
   	        {
   	                TRACE(TRL3,"Received packet at DTP-Host: %d\n",address());
	                ((DTPPacket*)pkt)->print();
	        }
	}
	//((DTPPacket*)pkt)->print_header();
	//cout<<term_bit<<endl;
	write=0;
	flag=0;
	ECN=0;
	ECN=((DTPPacket*)pkt)->ECN;
	
	if(!sender)
	{
	if(((DTPPacket*)pkt)->id==recv_so_far+1)
	{
	        recv_so_far+=1;
	        write=1;
	}
	}
	else
        	recv_so_far=((DTPPacket*)pkt)->id;
	
	if(term_bit==0)
	{
		if (((DTPPacket*)pkt)->sync_bit==1&&sync_bit==1)
		{
			get_dest(pkt);
			sync_bit=2;
			TRACE(TRL4,"(Time:%d) node %d Got SYN from node %d\n",scheduler->time(),address(),destination);
			handle_timer((void*)1);
			retrans=scheduler->time() +RTO;
			set_timer(retrans, NULL);
			TRACE(TRL4,"Next Retransmit at DTP-Host %d is set at Time: %d\n",address(),retrans);
			//cout<<"Next Retransmit:"<<retrans<<endl;
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
			RTT=(scheduler->time()-last_transmit);
			RTTVAR=RTT/2;
			RTO = (RTT + K*RTTVAR);
			last_ack=2;
			TRACE(TRL4,"(Time:%d) node %d Got SYN-ACK from node %d\n",scheduler->time(),address(),destination);
			flag=1;
			//fprintf(stderr,"Got SYN, node %d, %d %d\n",address(),RTO,last_transmit);
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
			sync_bit=3;
			get_dest(pkt);
			Time tm=scheduler->time();
			TRACE(TRL4,"(Time:%d) node %d Got ACK from node %d\n",scheduler->time(),address(),destination);
			    
			TRACE(TRL3, "Established FDTP flow from %d to %d (%d)\n",  destination,address(),tm);
			flag=1;
			if(((DTPPacket*)pkt)->id>recv_so_far)
	                {
	                        flag=0;
	                        recv_so_far+=1;
	                        if(((DTPPacket*)pkt)->id==recv_so_far+1)
                                {
                                        recv_so_far+=1;
	                                write=1;
	                        }
	                        //handle_timer((void*)1);
	                }
			//out_file.open("example.txt", ios::out);
			//term_bit=1;
				
			/* 	Node* nd = (scheduler->get_node)(destination);
			((Host*)nd)->terminate(address()); */
			
		}

	
	}
	
	if(sync_bit==3&&!flag)
	{			
	        if(((DTPPacket*)pkt)->FIN==0)
		{
		        if(!sender)
        		{
        		       // cout<<"khg"<<endl;
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
        	}
		else if(((DTPPacket*)pkt)->FIN==1&&((DTPPacket*)pkt)->ACK==0)
		{
		     
		        if(!sender)
		        {
		                if(1)//term_bit!=2)
		                {       
                		        if(((DTPPacket*)pkt)->id==recv_so_far)
                		        {
                		                TRACE(TRL3,"Received packet at DTP-Host: %d\n",address());      
                                	        ((DTPPacket*)pkt)->print();
                	                        DTPPacket*	pkt1 = new DTPPacket;
                                                pkt1->source = address();
                                                pkt1->destination = destination;
                                                pkt1->length = HEADER_SIZE;
                                                //if(write)
                                                        sent_so_far+=1;
                                                pkt1->id = sent_so_far;
                                              	pkt1->ack_id =recv_so_far;
                                        	pkt1->sync_bit=sync_bit;
                                        	pkt1->FIN=1;
                                                pkt1->ACK=1;
                                                pkt1->ECN=0;
                                                pkt1->ECN1=ECN; 
                        	                if (send(pkt1)) 
                                                {
                                                        TRACE(TRL3,"Sent packet from DTP-Host: %d\n",address());
                                                        pkt1->print();//_header();
                                                }
	                                        TRACE(TRL4,"(Time:%d) node %d Sent FIN-ACK to node %d\n",scheduler->time(),address(),destination);
                	                        //term_bit=2;
                	                        //terminate(destination);
                		        }
                		        else
                		        {
                		                term_bit=1;
                		                ter_seq=((DTPPacket*)pkt)->id;
                		                recv_window_sync((DTPPacket*)pkt);
                		        }
        		        }
        	      }
		      /*else
		      {
		        sent_window_sync((DTPPacket*)pkt);
		      }
		     */
                }
                else if(((DTPPacket*)pkt)->FIN==1&&((DTPPacket*)pkt)->ACK==1)
	        {
	            // cout<<"aaa"<<endl;
	            
	             if(sender)
		     {
		         if(term_bit==1)
		         {
		                term_bit=2;
		                sent_window_sync((DTPPacket*)pkt);
		                TRACE(TRL3, "Tore down FDTP flow from %d to %d (%d)\n", address(),destination,scheduler->time());
		                Node* nd = (scheduler->get_node)(destination);
		                ((Host*)nd)->term_bit=2;
		                 //cout<<"ccc"<<term_bit<<endl;
		               // sent_window_sync((DTPPacket*)pkt);
		                
		          }
		       }
		 }
        }     
	
 //((DTPPacket*)pkt)->print();	
   if(pkt)
   delete pkt;
} 


void
Host::handle_timer(void* cookie)
{
    
	DTPPacket*	pkt_1 = new DTPPacket;

	//New Connection Setup//
	if((sync_bit==1&&term_bit==0)||(sync_bit==3&&term_bit==2))
	{
	        SendMap_iter head = dest_map.find(scheduler->time());
		if(head!=dest_map.end())
		{
		        if(retrans==0)
        		{	
        		        sync();
        			last_transmit=scheduler->time();
        			retrans=scheduler->time() +RTO;
        			set_timer(retrans, NULL);
        			TRACE(TRL4,"Next Retransmit at DTP-Host %d is set at Time: %d\n",address(),retrans);
        			retrans_bit=1;
        			TRACE(TRL4,"(Time:%d) node %d Sent SYN to node %d\n",scheduler->time(),address(),destination);
        		}
		}
	}
        
        //Retransmit of Data Packet//
        if(sync_bit==3)
	{
		if(retrans_bit==2)
		{	
			//cout<<"kk";
			//last_transmit=scheduler->time();
			if(retrans==scheduler->time())
			{
			        back_off_counter=back_off_counter*2;
        			retrans=scheduler->time() +back_off_counter*RTO; ////Exponential Backoff////
        			set_timer(retrans, NULL);
        			TRACE(TRL4,"Next Retransmit at DTP-Host %d is set at Time: %d\n",address(),retrans);
        			retrans_bit=2;
        			//cout<<"kk";
        			DTPPacket*	pkt_2 = new DTPPacket;
        			copy_pkt(pkt_2,retransmit_pkt);
        			pkt_2->ack_id =recv_so_far;
        			pkt_2->ECN=0;
        			pkt_2->ECN1=ECN;
        			if (send(pkt_2)) 
        			{
                                        TRACE(TRL3,"Retransmit packet from DTP-Host: %d\n",address());
	                                pkt_2->print();//_header();
	                        }
	                        Window_threshold=window_size/2;
        	                if(Window_threshold<1)
                                        Window_threshold=1;
                                window_size=1;
        	                TRACE(TRL4,"Current Window Size at DTP-Host: %d is %f\n",address(),window_size);
	                }
	                //cout<<"kk";
	                goto timer_end;
		}
	}
        
        
        //char* d = &(pkt->data[0]);
        // pkt->FIN=term_bit;
        pkt_1->FIN=0;
        pkt_1->ACK=0;
        pkt_1->ECN=0;
        pkt_1->ECN1=ECN;
        if(term_bit==2&&!sender)
        {
                pkt_1->FIN=1;
                pkt_1->ACK=1;
        }
        pkt_1->source = address();
        pkt_1->destination = destination;
        pkt_1->length = HEADER_SIZE;
        sent_so_far+=1;
        
        //Retransmit of Connection setup Packet
	if(retrans==scheduler->time()&&retrans_bit==1)
	{
                sent_so_far-=1;
		retrans=scheduler->time() +RTO;
		set_timer(retrans, NULL);
		TRACE(TRL4,"Next Retransmit at DTP-Host %d is set at Time: %d\n",address(),retrans);
		//TRACE(TRL3,"RETRANSMIT");
		pkt_1->id = sent_so_far;
	        pkt_1->ack_id =recv_so_far;
         	pkt_1->sync_bit=sync_bit;
                if (send(pkt_1)) 
                {
                       TRACE(TRL3,"Retransmit packet from DTP-Host: %d\n",address());
                       pkt_1->print();//_header();
                }
                goto timer_end;
	} 
	
	
	//For Connection Setup and Acks
	
	//fprintf(stderr,"\n %d",sent_so_far)
	pkt_1->id = sent_so_far;
	pkt_1->ack_id =recv_so_far;
 	pkt_1->sync_bit=sync_bit;
        if (send(pkt_1)) 
        {
                TRACE(TRL3,"Sent packet from DTP-Host: %d\n",address());
	        pkt_1->print();//_header();
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
	TRACE(TRL4,"The new file to be transfered from DTP-Host %d is: %s\n",address(),newpair->name);
	//cout<<newpair->name<<endl;
	in_file.open (newpair->name, ios::in); 
	
	
	//For creating a blank dest file
	Node* nd = (scheduler->get_node)(destination);
	char name[99999];
	strcpy(name,"dest_");
        strcat(name,newpair->name);
        strcpy((((Host*)nd)->out_file),name);
	ofstream file1(name, ios::out);
	file1.close();
	//For creating a blank dest file
	
	
	reset();
	((Host*)nd)->reset();
	sender=1;
	
}



void
Host::send_file()
{
        char line[PAYLOAD_SIZE];
        char c;
        int i=0;
	if (in_file.is_open())
        {       
                while (!in_file.eof())
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
        	                //fprintf(stderr,"%02x ", (unsigned char)line[i]);
        	                i++;
        	                if(i==PAYLOAD_SIZE-1)
        	                        break;
        	        }
        	        if(in_file.eof())
        	               i-=1;
        	        line[i]='\0';    
        	        //cout<<i<<endl;
 
                        DTPPacket* pkt_s= new DTPPacket;
                        unsigned char* d;
                        sent_so_far+=1;
        
                        d = &(pkt_s->data[0]);
                        for (int ll = 0; ll <= i; ll++) 
                        {
                                *(d+ll) = (unsigned char) line[ll];	
                        }
                        //strcpy(d,line);
        	        //cout<<d<<endl;
        	        
                        pkt_s->FIN=0;
                        pkt_s->ACK=0;
                        pkt_s->ECN=0;
        		pkt_s->ECN1=ECN;
                        pkt_s->source = address();
                        pkt_s->destination = destination;
                        pkt_s->length = HEADER_SIZE+i;
        	        pkt_s->id = sent_so_far;
        	        pkt_s->ack_id =recv_so_far;
         	        pkt_s->sync_bit=sync_bit;
         	        
         	        //Save a copy
         	        DTPPacket* pkt11= new DTPPacket;
         	        copy_pkt(pkt11,pkt_s);
         	        
         	        if (send(pkt_s)) 
                        {
                                TRACE(TRL3,"Sent packet from DTP-Host: %d\n",address());
        	                pkt_s->print();
        	        }
        	        {
        	                Window1Pair entry(pkt11->id,pkt11);
        	                sent_window.insert(entry); 
        	                id_timePair entry_time(pkt11->id,scheduler->time());
        	                send_time.insert(entry_time) ;
        	        } 
                }
                in_file.close();
                //terminate(destination);                
        }
        
        if(packets_in_window<window_size&&term_bit==0)
        {    
                terminate(destination);
        }
        
ss:     Window1_iter head1 =sent_window.begin();
        if(!sent_window.empty())
	{
	        retransmit_pkt=(*head1).second;
	        //cout<<"NEW RETRANS PACKET:"<<endl<<endl;
	        //retransmit_pkt->print_header();
	}
        else
                retransmit_pkt=NULL;


        if(retrans_bit==0&&retransmit_pkt!=NULL)
	{
	        back_off_counter=1;
	        last_transmit=scheduler->time();
	        retrans_bit=2;
	       	retrans=scheduler->time() +RTO;
	       	set_timer(retrans, NULL);
	       	TRACE(TRL4,"Next Retransmit at DTP-Host %d is set at Time: %d\n",address(),retrans);
	}
	//display();		        
        return;
}

void Host:: recv_window_sync(DTPPacket* pkt)
{
      	DTPPacket* pkt_recv=new DTPPacket;
        copy_pkt(pkt_recv,pkt);//cout<<"aaaaaa"<<endl;
      	if((pkt_recv->id)<recv_so_far)
	{
	        TRACE(TRL3,"Received an old packet at DTP-Host: %d\n",address());
	        //((DTPPacket*)pkt)->print();
	                //handle_timer((void*)1);
	}
	else
	{
	Window1_iter head = recv_window.find(pkt_recv->id);
      	ofstream file1(out_file, ios::out | ios::app| ios::binary);
	if(head!=recv_window.end())
	{
	        //handle_timer((void*)1);
	}
	else if((pkt_recv->id)==recv_so_far&&write)
	{
	       
	        //cout<<"aa:"<<pkt_recv->data<<endl;
	        TRACE(TRL3,"Received packet at DTP-Host: %d\n",address());
	        ((DTPPacket*)pkt)->print();
	        //int i=0;
	        if (file1.is_open())
	        {
	               int i=0;
	               int leng=(pkt_recv->length)-HEADER_SIZE;
	               // cout<<"hh"<<endl;
	               for(i=0;i<leng;i++)
	               {
	                 file1.put(pkt_recv->data[i]);
	               //  i++;
	               }
	        }
                
	        if(!recv_window.empty())
	        {
	        Window1_iter head1 =recv_window.begin();
	        //display(recv_window);
	        while(head1!=recv_window.end())
	        {
	                DTPPacket *pkt_iter_recv;
	                pkt_iter_recv=(*head1).second;
	                if(pkt_iter_recv->id!=recv_so_far+1)
	                {
	                        break;
	                }
	                recv_so_far+=1;
	                TRACE(TRL3,"Received packet at DTP-Host: %d\n",address());
	                ((DTPPacket*)pkt_iter_recv)->print();
	                if(pkt_iter_recv->FIN==1)
	                {
	                        recv_window.erase(head1);
	                        break;
	                }
	                int i=0;
	                int leng=(pkt_iter_recv->length)-HEADER_SIZE;
	               // cout<<"hh"<<endl;
	                for(i=0;i<leng;i++)
	                {
	                       file1.put(pkt_iter_recv->data[i]);
        	               //  i++;
                        }
	               
	                
	              // file1<<pkt_iter_recv->data;
	               // cout<<"bb"<<pkt_iter_recv->data<<endl;
	                recv_window.erase(head1);
	                head1 =recv_window.begin();
	                delete pkt_iter_recv;
	        }
	        }
	                
         }
         else if((pkt_recv->id)>recv_so_far)
         {      
                DTPPacket* pkt_store=new DTPPacket;
                copy_pkt(pkt_store,pkt_recv);
                TRACE(TRL3,"Received an Out-of-Order packet at DTP-Host: %d\n",address());
                Window1Pair entry(pkt_store->id,pkt_store);
	        recv_window.insert(entry);
	        //set_timer(retrans, NULL);
	 }   
	 
	
	// display(recv_window);
	 file1.close();
	 }
	 
	 delete  pkt_recv;
	 
	 if(ter_seq==recv_so_far+1&&write)
	 {
                DTPPacket*	ter_pkt = new DTPPacket;
                ter_pkt->source = address();
                ter_pkt->destination = destination;
                ter_pkt->length = HEADER_SIZE;
                sent_so_far+=1;
                ter_pkt->id = sent_so_far;
                recv_so_far+=1;
                ter_pkt->ack_id =recv_so_far;
                ter_pkt->sync_bit=sync_bit;
                ter_pkt->FIN=1;
                ter_pkt->ACK=1;
                ter_pkt->ECN=0;
                ter_pkt->ECN1=ECN;
                if (send(ter_pkt)) 
                {
                        TRACE(TRL3,"Sent packet from DTP-Host: %d\n",address());
                        ter_pkt->print();//_header();
                }
                TRACE(TRL4,"(Time:%d) node %d Sent FIN-ACK to node %d\n",scheduler->time(),address(),destination);
                term_bit=2;
	 }
	 else
	 {
	        handle_timer((void*)1);
	 }
}

void Host:: sent_window_sync(DTPPacket* pkt)
{
        //cout<<address()<<endl;
        //display();
        //cout<<"jddjdkdkd"<<endl;
        //cout<<"last_ack="<<last_ack<<endl;
        if(pkt->ack_id<last_ack)
        {
                //retransmit();
        }
        else if(pkt->ack_id==last_ack)
        {
                dup_ack_count+=1;
                //cout<<"DUP:"<<dup_ack_count<<endl;
                if((dup_ack_count%3)==0)
                {
                        //cout<<"DUP:"<<dup_ack_count<<endl;
                        window_size=window_size/2;
                        if(window_size<1)
                                window_size=1;
                        Window_threshold=window_size;
                        if(retrans_bit==2)
		        {	
			        if(retrans>scheduler->time())
			        {
			                cancel_timer(retrans, NULL);
			                retrans=scheduler->time() +RTO; 
			                set_timer(retrans, NULL);
			                TRACE(TRL4,"Next Retransmit at DTP-Host %d is set at Time: %d\n",address(),retrans);
			                DTPPacket *pkt_r= new DTPPacket;
			                copy_pkt(pkt_r,retransmit_pkt);
			                pkt_r->ack_id =recv_so_far;
			                pkt_r->ECN=0;
			                pkt_r->ECN1=ECN;
			                if (send(pkt_r)) 
			                {
                                                TRACE(TRL3,"Retransmit packet (Due to Three Dup-Acks) from DTP-Host: %d\n",address());
        	                                pkt_r->print();//_header();
        	                        }
        	               }
	                }
                }
        }
        else
        {//cout<<"jddjdkdkd"<<endl;
                //DTPPacket*	pkt10 = new DTPPacket;
	       // copy_pkt(pkt10,pkt);
	        dup_ack_count=0;
	        congestion_control(pkt);
	        
                if(retrans>=scheduler->time())
	        {
	                cancel_timer(retrans, NULL);
                        retrans=0;
                        retrans_bit=0;
                 }
	
	//cout<<"cccc"<<endl<<endl;
	//pkt->print_header();
        last_ack=pkt->ack_id;
        DTPPacket *pkt_send;
        Window1_iter head = sent_window.find(pkt->ack_id);
	
	if(head!=sent_window.end()&&!sent_window.empty())
	{
	        //cout<<"jddjdkdkd"<<endl;
	        Window1_iter head1 =sent_window.begin();
	        if(head1==head)
	        {
	                pkt_send=(*head1).second;
	                //cout<<"bbbb"<<endl<<endl;
	                //pkt_send->print_header();
	               // last_ack++;
	                sent_window.erase(head1);
	                packets_in_window--;
	        }
	        else
	        {
	        while(head1!=head)
	        {
	                head1 =sent_window.begin();
	                pkt_send=(*head1).second;
	                //cout<<"bbbb"<<endl<<endl;
	                //pkt_send->print_header();
	                // last_ack++;;
	                sent_window.erase(head1);
	                packets_in_window--;
	        }
	        }
	        //update window
	        //retransmit
	}
	
	//display();
	
	RTTVAR=(1 - beta)*RTTVAR +beta*abs(RTT - (scheduler->time()-last_transmit));
	
	RTT=(1 - alpha) * RTT + alpha *(scheduler->time()-last_transmit);
	
	RTO = (RTT + K*RTTVAR);
	TRACE(TRL4,"Current RTT: %d\n Current RTO: %d",scheduler->time()-last_transmit,RTO);
        id_time_iter id_timer = send_time.find(pkt->ack_id+1);

	                //cout<<"kk";	
	if(id_timer!=send_time.end()&&!send_time.empty())
	{
	        last_transmit=(*id_timer).second;
	}
	else
	{
	        last_transmit=scheduler->time();	
	}
	//cout<<"last_transmit:"<<last_transmit<<endl;           
	        
	Window1_iter head1 =sent_window.begin();
	if(head1!=sent_window.end())
	        retransmit_pkt=(*head1).second;
	else
                retransmit_pkt=NULL;
                
        if(retrans_bit==0&&retransmit_pkt!=NULL)
        {
              	
	        back_off_counter=1;
	        retrans_bit=2;
               /* if((last_transmit+RTO)>scheduler->time())
                        retrans=last_transmit+RTO;
                else*/
                
                retrans=scheduler->time() +RTO;
                set_timer(retrans, NULL);
                if(retransmit_pkt->FIN!=1&&((last_transmit+RTO)<scheduler->time()))
              	{
              	        DTPPacket*	pkt9 = new DTPPacket;
        	        copy_pkt(pkt9,retransmit_pkt);
        	        pkt9->ack_id =recv_so_far;
        	        if (send(pkt9)) 
        	        {
                                TRACE(TRL3,"Retransmit packet from DTP-Host: %d\n",address());
        	                pkt9->print();//_header();
        	        }
	        }
                TRACE(TRL4,"Next Retransmit at DTP-Host %d is set at Time: %d\n",address(),retrans);
        }       
        }
        //cout<<"last_ack="<<last_ack<<endl;
        return;
}
void 
Host::congestion_control(DTPPacket* pkt)
{
        //int i=window_size;
        TRACE(TRL4,"Previous Window Size at DTP-Host: %d is %f,  thres= %f\n",address(),window_size,Window_threshold);
        if(ACKcount_flag)
        {
                if(ACKcounter>0)
                {
                        ACKcounter--;
                        if(pkt->ECN1==1)
                        {
                               ECNcount++;
                        }
               }
                else
                        ACKcount_flag=0;
                        
        }
        else
        {
                if(ACKcount)
                alpha1=alpha1*(1-G)+G*((float)ECNcount)/((float)ACKcount);
                //cout<<"alpha1"<<alpha1<<endl;
                ACKcount=floor(window_size);
                ACKcounter=ACKcount;
                ECNcount=0;
                ACKcount_flag=1;
                window_size=window_size*(1-alpha1/2);
        }
        
        
        int j=(pkt->ack_id)-last_ack;
        if(window_size+j<Window_threshold)
        {
                window_size=window_size+j;
        }
        else if(window_size<Window_threshold&&Window_threshold<=window_size+j)
        {
                window_size=Window_threshold;
        }
        else if(Window_threshold<=window_size)
        {
                window_size=window_size+((float)j)/window_size;
                //cout<<"ff"<<Window_threshold<<endl;
        }
        else;
        /*if(pkt->ECN1==1)
        {
                if(ACKcount_flag)
                        ECNcount++;
                window_size=window_size/2;               
        }
        else
        {
                int j=(pkt->ack_id)-last_ack;
               // last_ack;
                //cout<<"aa"<<Window_threshold<<"dd"<<j<<endl;
                if(window_size+j<Window_threshold)
                {
                        window_size=window_size+j;
                }
                else if(window_size<Window_threshold&&Window_threshold<=window_size+j)
                {
                        //cout<<"gg"<<endl;
                        window_size=Window_threshold;
                }
                else if(Window_threshold<=window_size)
                {
                        window_size=window_size+((float)j)/window_size;
                        //cout<<"ff"<<Window_threshold<<endl;
                }
                else;
                
                
        }*/
        if(window_size<1)
                window_size=1;
        
        TRACE(TRL4,"Current Window Size at DTP-Host: %d is %f ,thre %f\n",address(),window_size,Window_threshold);

        return;
}

void 
Host::display(Window1 w1)
{
        
        
        if(!w1.empty())
	{
	        //cout<<"jddjdkdkd"<<endl;
	        Window1_iter head1 =w1.begin();
	        DTPPacket* pkt1;
	        while(head1!=w1.end())
	        {
	                pkt1=(*head1).second;
	                //cout<<"dddd"<<endl<<endl;
	                pkt1->print();//_header();
	                head1=w1.upper_bound(pkt1->id);
	        }
	}
}
void Host::copy_pkt(DTPPacket* pkt_to,DTPPacket* pkt_from)
{
        pkt_to->FIN=pkt_from->FIN;
        pkt_to->ACK=pkt_from->ACK;
        pkt_to->source = pkt_from->source;
        pkt_to->destination = pkt_from->destination;
        pkt_to->length = pkt_from->length;
        pkt_to->id = pkt_from->id;
        pkt_to->ack_id =pkt_to->ack_id;
        pkt_to->sync_bit=pkt_from->sync_bit; 
        int i=0;
        int leng=(pkt_from->length)-HEADER_SIZE;
	               	                
	unsigned char *a1,*a2;
        a1=&(pkt_from->data[0]);
        a2=&(pkt_to->data[0]);
        for(i=0;i<=leng;i++)
        {
                *(a2+i)=*(a1+i);
                //i++;
        }
       // strcpy(pkt_to->data,pkt_from->data);
        pkt_to->ECN=pkt_from->ECN;
        pkt_to->ECN1=pkt_from->ECN1;
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
	
        destination = d;
	term_bit=1;
	 //handle_timer((void*)1);
	DTPPacket*	pkt12 = new DTPPacket;
        pkt12->source = address();
        pkt12->destination = destination;
        pkt12->length = HEADER_SIZE;
        sent_so_far+=1;
        pkt12->id = sent_so_far;
	pkt12->ack_id =recv_so_far;
 	pkt12->sync_bit=sync_bit;
 	pkt12->FIN=1;
        pkt12->ACK=0;
        pkt12->ECN=0;
        pkt12->ECN1=ECN;
        DTPPacket*	pkt11 = new DTPPacket;
        copy_pkt(pkt11,pkt12);
        Window1Pair entry(pkt11->id,pkt11);
        sent_window.insert(entry);
        if (send(pkt12)) 
        {
                TRACE(TRL3,"Sent packet from DTP-Host: %d\n",address());
	        pkt12->print();//_header();
	}

	TRACE(TRL4,"(Time:%d) node %d Sent FIN to node %d\n",scheduler->time(),address(),destination);
			
}
