#define CONFIG_MAX_ARGS		20	// Number of args 
#define CONFIG_MAX_IDLEN	50	// Length of task string

union ArgType {
    char* 	strval;
    int 	numval;
};

//
// The Config class encapsulates config file processing.
// A config file typically consists of a sequence of commands,
// optionally interspersed with comments. See lexer.l and parser.cpp
// to understand how lex and yacc are used for parsing commands.
// Each command takes one or more integer arguments. See the
// "cbr_apps/examples" directory for examples of the configuration
// syntax. 

class Config {

 public:
    Config();
    ~Config();

    void add_arg(char* s);			// string argument
    void add_arg(int a);			// add this arg
    void process_command(char* keyword);	// command keyword
    void parse(char* file);			// config file

 private:
    // For user-defined commands
    virtual void process_app_command(char* keyword);
    
    union ArgType config_args[CONFIG_MAX_ARGS];   // array of arguments
    int config_argnum; 			// number of args
};

extern Config* config;
