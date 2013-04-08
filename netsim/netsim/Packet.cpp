#include "common.h"
#include "Timer.h"
#include "Packet.h"
#include "PacketScheduler.h"
#include "Scheduler.h"
#include "Topology.h"
#include "Node.h"

void
Packet::print_header()
{
    TRACE(TRL3, "source: %d, destination: %d, id: %d, length: %d\n",
          (int) source, (int) destination, id, length);
}

void
Packet::print_payload(char *p,
                      unsigned int len,
                      bool ascii)
{
    if (trace & TRL3) {
        if (!ascii) {
            fprintf(stderr, "\t");
            for (int i = 1; i <= len; i++) {
                fprintf(stderr, "%02x ", (unsigned char) *(p+i-1));
                if (i % 8 == 0) {
                    fprintf(stderr, "\n\t");
                }
            }
        } else {
            for (int i = 0; i < len; i++) {
                fprintf(stderr, "%c", *(p+i));
            }
        }
        fprintf(stderr, "\n");
    }
}
