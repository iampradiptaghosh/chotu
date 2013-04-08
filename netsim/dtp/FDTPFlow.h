//#include "Host.h"
/* #include "../netsim/PacketScheduler.h"
class PacketScheduler; */
class FDTPFlow  {
        public:
                FDTPFlow(Address a,		// Unique node address
                Address d,		// Target
                Time s,			// Start time of node
                char* f);			// Number of packets to send
                ~FDTPFlow();
	
                // Handle a timer
	
};
