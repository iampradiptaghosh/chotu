#include <cstdio>
#include <sys/file.h>
#include <string.h>
#include "Config.h"
#include "../netsim/common.h"
#include "../netsim/Packet.h"
#include "../netsim/Timer.h"
#include "../netsim/PacketScheduler.h"
#include "../netsim/Scheduler.h"
#include "../netsim/Node.h"
#include "../netsim/FIFONode.h"
#include "../netsim/Topology.h"

// Global configuration object
Config *config;

// Extern file handle
extern FILE* yyin;
extern "C" int yyparse(void);

Config::Config()
{
    config_argnum = 0;
}

Config::~Config()
{
    // Empty
}

void
Config::add_arg(char* arg)
{
    config_args[config_argnum].strval =  strdup(arg); 
    config_argnum++;
}

void
Config::add_arg(int arg)
{
    config_args[config_argnum].numval = arg;
    config_argnum++;
}

void
Config::parse(char* config_file)
{
    yyin = fopen(config_file, "r");
    if (!yyin) {
        FATAL("Couldn't open config file");
    }
    yyparse();
    TRACE(TRL1, "Completed parsing config file: %s", config_file);
    return;
}

void
Config::process_command(char* id)
{
    if (strcmp(id, "Link") == 0) {
        if (config_argnum != 4) {
            fprintf(stderr, "Incorrect number of args for: %s", id);
        }
        topology->add_link((Address) config_args[0].numval,
                           (Address) config_args[1].numval,
                           (Time) config_args[2].numval,
                           (Time) config_args[3].numval);
    } else if (strcmp(id, "FIFONode") == 0) {
        if (config_argnum != 2) {
            fprintf(stderr, "Incorrect number of args for: %s", id);
        }
        FIFONode* f = new FIFONode((Address) config_args[0].numval,
                                   (int) config_args[1].numval);
    } else {
        process_app_command(id);
    }
    config_argnum = 0;
    return;
}


