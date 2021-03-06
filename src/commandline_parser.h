/*
 ============================================================================
 Name        : commandline_parser.h
 Author      : Christoph White
 Version     :
 Copyright   :
 Description  This is the commandline interface for aubatch. I used Dr. Qin's commandline_parser.c
 as a reference to write this. When this program exits with the quit command, the
 parent process that created this thread will kill all of it's children thread except
 the currently running job.
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
int cmd_list(int nargs, char **args);
int cmd_bench(int nargs, char **args);
int rand_range_gen(int low, int high);
int switch_priority(int nargs, char **args);
int switch_fcfs(int nargs, char **args);
int switch_sjf(int nargs, char **args);

/*
 * Error Codes.
 * Taken from Dr. Qin's "commlandline_parser.c" example
 */

#define EINVAL       1
#define E2BIG        2

#define MAXMENUARGS  7
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
	{ "test", cmd_bench },
	{ "fcfs\n", switch_fcfs },
	{ "sjf\n", switch_sjf },
	{ "priority\n", switch_priority },
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
	"[list] Display the job status       ",
	"[fcfs] Change the scheduling policy to FCFS.",
	"[sjf] Change the scheduling policy to SJF.",
	"[priority] Change the scheduling policy to priority.",
	"[test] <benchmark> <policy> <num_of_jobs> <priority_levels> <min_CPU_time> <max_CPU_time>",
        /* Please add more menu options below */
	NULL
};

char *otherargs[MAXMENUARGS];
int policy;







#endif /* COMMANDLINE_PARSER_H_ */
