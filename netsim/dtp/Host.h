#include <fstream>
class FIFONode;
class DTPPacket;
#define MAX 99999;
typedef struct 
{
    Address a;
    char name[1000];
}sendpair;
struct ltaddr {
    bool operator() (int t1, int t2) const {
        return (t1 < t2);
    }
};
typedef map< Time,sendpair*, ltTime> SendMap;
typedef map< Time,sendpair*, ltTime>::iterator SendMap_iter;
typedef pair<Time, sendpair*> SendMapPair;



typedef map< int,DTPPacket*,ltaddr> Window1;
typedef map< int,DTPPacket*,ltaddr>::iterator Window1_iter;
typedef pair<int,DTPPacket*> Window1Pair;

typedef map<int,Time,ltaddr> id_time;
typedef map< int,Time,ltaddr>::iterator id_time_iter;
typedef pair<int,Time> id_timePair;


class Host : public FIFONode {
 public:
    Host(Address a);
    ~Host();
        void get_dest(Packet* pkt);
	void handle_timer(void* cookie);
    // Receive packet and prints its length
    //void sync(Packet* pkt);
	void receive(Packet* pkt);
	void send_file();			// Incoming packet
	void sync();
	void reset();
	void sent_window_sync(DTPPacket* pkt);
	void recv_window_sync(DTPPacket* pkt);
	void insert_p(Time s,Address d,char* f);
	void terminate(Address d);
	void display(Window1 w1);
	void congestion_control(DTPPacket* pkt);
	void copy_pkt(DTPPacket* pkt_to,DTPPacket* pkt_from);
	
	
	int     dup_ack_count;
	int     sync_bit;
	int     term_bit;
	int     retrans_bit;
	int     packets_in_window;
	float   window_size;
	DTPPacket* retransmit_pkt;
	SendMap dest_map;
	ifstream in_file;
	char    out_file[1000];
	bool    sender;
	bool    ECN;
	int     Window_threshold;
	Window1 sent_window;	
        Window1 recv_window;
        id_time send_time;
        int     ECNcount;
        int     ACKcount;
        int     ACKcounter;
        bool    ACKcount_flag;
        float   alpha1;
        long int File_size;
 private:
        Time    last_transmit;
        Address	destination;		// Target address
        Time	start;			// Start sending at
        Time	retrans;	// Inter-packet time
        int	sent_so_far;
        int	recv_so_far;
        bool    write;
        int     last_ack;
        int	ter_seq;
        bool    flag;
        Time    RTO,RTT,RTTVAR;
    	
	
    
};
