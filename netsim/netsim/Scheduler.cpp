#include "common.h"
#include "Node.h"
#include "PacketScheduler.h"
#include "Timer.h"
#include "Scheduler.h"

// Global variable
Scheduler* scheduler;

Scheduler::Scheduler()
{
    virtual_clock = 0;
}

Scheduler::~Scheduler()
{
    // empty
}

// Return the current time
Time
Scheduler::time()
{
    return virtual_clock;
}

// Set the virtual clock to the specified time
// Do NOT call this unless you know what you're doing
void
Scheduler::set_clock(Time t)
{
    virtual_clock = t;
    TRACE(TRL1, "Updated the simulator clock to %d\n", virtual_clock);
}

// Run the discrete-event simulation
void
Scheduler::run()
{
    Time tt, tp;

    while (1) {
        tt = next_timer();
        tp = next_packet();

        if ((tt == -1) && (tp == -1)) { // We are done
            TRACE(TRL1, "No more events, done!\n");
            return;
        }

        if (tt == -1) {
            process_packet();
        } else if (tp == -1) {
            process_timer();
        } else if (tt <= tp) {
            process_timer();
        } else {
            process_packet();
        }
    }
}
