
class Node;
class DES;

//
// Like PacketData, TimerData is used to remember when to fire a timer
// and what "Node" to callback when the timer fires. The "cookie" is
// simply a number that the node can use to remember which timer was
// fired.
//

class TimerData {

    friend class Timer;

 private:
    Node* 	node;
    void*	cookie;

 public:
    TimerData(Node* n, void* c);
    ~TimerData();
};

//
// The TimeMap is an STL map that maintains a mapping between
// TimerData and the time at which the corresponding timer should be
// fired. This TimerMap is sorted in increasing order of time, so that
// Timer below can fire timers at the appropriate times.
//

typedef multimap<Time, TimerData*, ltTime> TimerMap;
typedef multimap<Time, TimerData*, ltTime>::iterator TimerMapIterator;
typedef pair<Time, TimerData*> TimerMapPair;

//
// The Timer class implements the timer management functions. It
// maintains a cue of time is to be fired in a TimerMap, and invokes
// each timer when the system virtual clock has advanced
// appropriately.
//

class Timer  {

    friend class DES;

public:
    Timer();
    ~Timer();

    // Set a timer for a specified virtual time
    // The node class implements a virtual function which is called
    // with the cookie as argument when the timer fires
    void set_timer(Time time,		// The absolute time
                   void* cookie,	// Cookie returned upon callback
                   Node* node);         // Pointer to the node class
                                        // that sets the time

    // Cancel a specified timer
    // Note: For this to work, different timers from the same
    // node must have different cookies
    void cancel_timer(Time time,	// The absolute time
                      void* cookie,	// A unique cookie for the timer
                      Node* node);	// Pointer to the node class
    					// that sets the time

    // Return the time corresponding to the timer at the head of the queue
    // If no timer exists, return -1
    Time next_timer();

    // Process the next timer from the queue
    // Side effect: this may update the global clock
    void process_timer();

private:
    TimerMap	timer_map;
};
