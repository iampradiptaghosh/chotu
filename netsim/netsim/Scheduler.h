
class Timer;
class PacketScheduler;

//
// The Scheduler class is at the heart of the discrete event
// simulator. Two types of events are defined in the system: timers
// and packets. Each of these events is handled in a separate class.
// The Scheduler maintains the all important "virtual clock".
//
// There is exactly 1 instance of the Scheduler class in the system.
//

class Scheduler : public Timer, public PacketScheduler {

public:
    Scheduler();
    ~Scheduler();

    // Start the simulation
    void run();

    // Return the current time
    Time time();

    // Advance the virtual time
    void set_clock(Time t);

private:
    // The simulation virtual time
    Time 		virtual_clock;
};

// Global variable
extern Scheduler* scheduler;
