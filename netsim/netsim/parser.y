%{
#include "Config.h"

extern "C"
{
    int yyparse(void);
    int yylex(void);  
    int yywrap();
    int yyerror(char*);
}
%}

%union {
    int val;
    char string[50];
}
%start config_commands
%token <string> identifier
%token <val> number
%token <string> quotedstr

%%
/* Arguments: integers or strings */

arglist: /* empty */
	| number
	{
#if CONFIG_DEBUG            
            fprintf(stderr, "In arg_list number rule: %d\n", $1);
#endif            
            config->add_arg((int) $1);
        }
	| quotedstr
        {
#if CONFIG_DEBUG            
            fprintf(stderr, "In arg_list string rule: %s\n", $1);
#endif            
            config->add_arg((char*) $1);
        }
	| arglist ',' number
	{
#if CONFIG_DEBUG            
            fprintf(stderr, "In arg_list rule: %d\n", $3);
#endif            
            config->add_arg((int) $3);
        }
	| arglist ',' quotedstr
	{
#if CONFIG_DEBUG            
            fprintf(stderr, "In arg_list rule: %d\n", $3);
#endif            
            config->add_arg((char*) $3);
        };

/* Task chain must have at least one element */
config_commands: config_command
	| config_command config_commands;

config_command: identifier '(' arglist ')'
	{
            config->process_command($1);
        };

%%


