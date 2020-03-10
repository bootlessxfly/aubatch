/*
 ============================================================================
 Name        : commandline_parser.h
 Author      : Christoph White
 Version     :
 Copyright   :
 Description  This is the commandline interface for aubatch. I used Dr. Qin's commandline_parser.c
 as a reference to write this
 ============================================================================
 */

#ifndef COMMANDLINE_PARSER_H_
#define COMMANDLINE_PARSER_H_

// Function declarations
int run_interface();
int cmd_dispatch(char *cmd);
void menu_execute(char *line, int isargs);
int cmd_run(int nargs, char **args);
int cmd_quit(int nargs, char **args);
void showmenu(const char *name, const char *x[]);
int cmd_helpmenu(int n, char **a);
int cmd_list();

/*
 * Error Codes.
 * Taken from Dr. Qin's "commlandline_parser.c" example
 */

#define EINVAL       1
#define E2BIG        2

#define MAXMENUARGS  4
#define MAXCMDLINE   64

//int schedular_policy = 0; //Defaults to FCFS

/*
 *  Command table.
 *  Taken from Dr. Qin's "commlandline_parser.c" example
 */
static struct {
	const char *name;
	int (*func)(int nargs, char **args);
} cmdtable[] = {
	/* commands: single command must end with \n */
	{ "?\n",	cmd_helpmenu },
	{ "h\n",	cmd_helpmenu },
	{ "help\n",	cmd_helpmenu },
	{ "r",		cmd_run },
	{ "run",	cmd_run },
	{ "q\n",	cmd_quit },
	{ "quit\n",	cmd_quit },
	{ "list\n", cmd_list },
        /* Please add more operations below. */
        {NULL, NULL}
};

/*
 * Help Menu String.
 * Taken from Dr. Qin's "commlandline_parser.c" example
 */

static const char *helpmenu[] = {
	"[run] <job> <time> <priority>       ",
	"[quit] Exit AUbatch                 ",
	"[help] Print help menu              ",
	"[list] display the job status       ",
        /* Please add more menu options below */
	NULL
};

char *otherargs[MAXMENUARGS];
int policy;







#endif /* COMMANDLINE_PARSER_H_ */
