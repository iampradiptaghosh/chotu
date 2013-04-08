#include "common.h"
#include "Timer.h"
#include "Node.h"
#include "PacketScheduler.h"
#include "Topology.h"
#include "Scheduler.h"

TimerData::TimerData(Node* n,
                     void* c)
{
    node = n;
    cookie = c;
}

TimerData::~TimerData()
{
    // Empty
}

Timer::Timer() 
{
    // Empty
}

Timer::~Timer()
{
    // Empty
}

// Set timer
void
Timer::set_timer(Time time,
                 void* cookie,
                 Node* node) 
{
    TimerData* 	t = new TimerData(node, cookie);
    TimerMapPair entry(time, t);

    TRACE(TRL1, "Setting a timer for time %d at node %d\n",
          time, node->address());
    timer_map.insert(entry);
}

// Cancel timer
void
Timer::cancel_timer(Time time,
                    void* cookie,
                    Node* node)
{
    TimerMapIterator	ti = timer_map.lower_bound(time);
    TimerData*		td;

    while (ti != timer_map.upper_bound(time)) {
        td = (*ti).second;
        if ((td->node == node) && (td->cookie == cookie)) {
            timer_map.erase(ti);
            return;
        }
        ti++;
    }

    // If we are here, cancel timer must have been called on a
    // non-existent timer, so die.
    ASSERT(false);
}

// Time of next event
Time
Timer::next_timer()
{
    if (timer_map.empty()) {
        return -1;
    }

    TimerMapIterator head = timer_map.begin();
    return (*head).first;
}

// Process the first event in the queue
void
Timer::process_timer()
{
    if (timer_map.empty()) {
        return;
    }

    TimerMapIterator head = timer_map.begin();
    TimerData* t = (*head).second;

    // Update the global time, then call the timer handler
    scheduler->set_clock((*head).first);
    timer_map.erase(head);

    TRACE(TRL1, "Calling timer at %d\n", t->node->address());
    t->node->handle_timer(t->cookie);

    delete t;
    return;
}
