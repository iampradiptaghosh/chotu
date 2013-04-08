#include <cstdio>
#include <iostream>
#include <sys/types.h>
#include <stdlib.h>
#include <map>
#include <queue>
using namespace std;

typedef unsigned short Address;
typedef int Time;

struct ltTime {
    bool operator() (Time t1, Time t2) const {
        return (t1 < t2);
    }
};

struct ltAddress {
    bool operator() (Address a1, Address a2) const {
        return (a1 < a2);
    }
};

#define ASSERT(x) {if (!(x)) {cerr << "Assertion failed, exiting"; exit(0);}}

// Tracing support
// - we define several trace levels
// - 1: scheduler and topology
// - 2: node packet processing
// - 3 and higher: application level
// The "trace" variable, whose value is specified in the command line,
// governs which statements get printed.

extern unsigned int trace;		// Defined in main.cpp

// Trace levels
#define TRL1	1
#define TRL2	2
#define TRL3	4
#define TRL4    8

#define TRACE(level, fmt, args...) \
    if (level & trace) {\
	fprintf(stderr, fmt, ## args); \
	fprintf(stderr, " <%d, %s>\n", scheduler->time(), __PRETTY_FUNCTION__); \
        fflush(stderr); \
    }

// Other printing support
#define FATAL(fmt, args...) {fprintf(stderr, fmt, ## args); exit(1);}

