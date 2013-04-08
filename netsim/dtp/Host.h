class FIFONode;
#define MAX 99999;

typedef map< Time,Address, ltTime> SendMap;
typedef map< Time,Address, ltTime>::iterator SendMap_iter;
typedef pair<Time, Address> SendMapPair;
class Host : public FIFONode {
 public:
        Host(Address a);
        ~Host();
        void get_dest(Packet* pkt);
	void handle_timer(void* cookie);
        // Receive packet and prints its length
        //void sync(Packet* pkt);
	void receive(Packet* pkt);		// Incoming packet
	void sync();
	void insert_p(Time s,Address d);
	void terminate(Address d);
	int sync_bit;
	int term_bit;
	int     retrans_bit;
	
	SendMap dest_map;
	//int dest[MAX];
	//int dest_count,dest_pointer;
 private:
	Time retrans_timer, last_transmit;
        Address	destination;		// Target address
        Time	start;			// Start sending at
        Time	retrans;	// Inter-packet time
        int packets_to_send;	// number of packets
        int sent_so_far;	
	
    
};
