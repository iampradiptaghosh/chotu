#include "../netsim/common.h"
#include "../netsim/Node.h"
#include "../netsim/FIFONode.h"
#include "../netsim/PacketScheduler.h"
#include "../netsim/Timer.h"
#include "../netsim/Scheduler.h"
#include "Router.h"
#include "FDTPFlow.h"
#include "Host.h"

Router::Router(Address a, int b) : FIFONode(a,b)	// Null queue size
{
    TRACE(TRL3, "Initialized router with address %d\n",a);// Empty
}

Router::~Router()
{
    // Empty
}
