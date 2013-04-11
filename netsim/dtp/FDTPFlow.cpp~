#include "../netsim/common.h"
#include "../netsim/Node.h"
#include "../netsim/FIFONode.h"
#include "../netsim/Packet.h"
#include "DTPPacket.h"
#include "../netsim/Timer.h"
#include "../netsim/PacketScheduler.h"
#include "../netsim/Scheduler.h"
#include "FDTPFlow.h"
#include "Host.h"
#include "../netsim/Topology.h"
#include "Router.h"
//typedef map<Address, Node*, ltAddress> NodeMap;

FDTPFlow::FDTPFlow(Address a, Address d,Time s, char* f)
{
   // NodeMap	node_map;
	Node* nd = (scheduler->get_node)(a);
	(((Host*)nd)->insert_p)(s,d);
	(((Host*)nd)->set_timer)(s,NULL);
//	(((Host*)nd)->sync)();
	/* while(!(((Host*)nd)->sync_bit))
		printf("\nff"); */
//	((Host*)nd)->terminate(d);
}

FDTPFlow::~FDTPFlow()
{
    // Empty
}
