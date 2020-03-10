/*
 ============================================================================
 Name        : commandline_parser.c
 Author      : Christoph White
 Version     :
 Copyright   :
 Description  This is the commandline interface for aubatch. I used Dr. Qin's commandline_parser.c
 as a reference to write this
 ============================================================================
 */

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "commandline_parser.h"
#include "jobs.h"
#include "schedular.h"


int cmd_bench(int nargs, char **args) {
	if (nargs != 7) {
		printf("usage: test <benchmark> <policy> <num_of_jobs> <priority_levels> <min_CPU_time> <max_CPU_time>\n");
		return EINVAL;
	}
}

int cmd_list(int nargs, char **args) {
//	char *policyname = malloc(7);
	int start;
	int end;
//	if (policy == FCFS_ID) {
//		strcpy(policyname, "FCFS");
//	}
	if (count == 0) {
		printf("Total number of jobs in the queue: %d\n", count);
		printf("Scheduling Policy: %s\n", policyname);
		printf("There are no jobs to display\n");
		return EXIT_SUCCESS;
	}
	else {
		printf("Total number of jobs in the queue: %d\n", count);
		printf("Scheduling Policy: %s\n", policyname);
		/*
		 * Printing the list can take a job name of size 15 before things start printing weirdly
		 */
		printf("Name            CPU_Time Pri Arrival_time Progress\n");
		start = run_head - 1;
		end = start + count;
		for (int i = start; i < end; i++) {
			struct job j = jobs[i];
			print_job(&j);
		}
		return EXIT_SUCCESS;
	}
}

void print_job(struct job *j) {
	int comp;
	printf(j->_jobname);
	comp = strlen(j->_jobname);

	for (int i = 0; i <= (15 - comp); i++) {
		printf(" ");
	}

	printf("%d", j->exectime);
	comp = j->exectime;
	for (int i = 0; i < 8; i++) {
		comp = comp / 10;
		if (comp < 1) {
			printf(" ");
		}
	}
	printf("%d", j->priority);
	if (j->priority >= 100) {
		printf(" ");
	}
	else if (j->priority >= 10) {
		printf("  ");
	}
	else {
		printf("   ");
	}
	printf(j->arrivalTimeString);
	printf("     ");
	if (j->status == 1) {
		printf("Running\n");
	}
	// This should probably not ever happen, but am adding this in case it does
	else if (j->status == 2){
		printf("Completed!\n");
	}
	else {
		printf("Waiting!\n");
	}

}

/*
 * The run command - submit a job.
 * Taken from Dr. Qin's "commlandline_parser.c" example
 */
int cmd_run(int nargs, char **args) {
	if (nargs != 4) {
		printf("Usage: run <job> <time> <priority>\n");
		return EINVAL;
	}
        /* Use execv to run the submitted job in AUbatch */
        //printf("use execv to run the job in AUbatch.\n");
	add_job(policy, otherargs);
	struct job j = jobs[job_head];
    return 0; /* if succeed */
}

/*
 * The quit command.
 * Taken from Dr. Qin's "commlandline_parser.c" example
 */
int cmd_quit(int nargs, char **args) {
	printf("Please display performance information before exiting AUbatch!\n");
        exit(0);
}

/*
 * Display menu information
 * Taken from Dr. Qin's "commlandline_parser.c" example
 */
void showmenu(const char *name, const char *x[])
{
	int ct, half, i;

	printf("\n");
	printf("%s\n", name);

	for (i=ct=0; x[i]; i++) {
		ct++;
	}
	half = (ct+1)/2;

	for (i=0; i<half; i++) {
		printf("    %-36s", x[i]);
		if (i+half < ct) {
			printf("%s", x[i+half]);
		}
		printf("\n");
	}

	printf("\n");
}

//static const char *helpmenu[] = {
//	"[run] <job> <time> <priority>       ",
//	"[quit] Exit AUbatch                 ",
//	"[help] Print help menu              ",
//        /* Please add more menu options below */
//	NULL
//};

int cmd_helpmenu(int n, char **a)
{
	(void)n;
	(void)a;

	showmenu("AUbatch help menu", helpmenu);
	return 0;
}

/*
 *  Command table.
 */
//static struct {
//	const char *name;
//	int (*func)(int nargs, char **args);
//} cmdtable[] = {
//	/* commands: single command must end with \n */
//	{ "?\n",	cmd_helpmenu },
//	{ "h\n",	cmd_helpmenu },
//	{ "help\n",	cmd_helpmenu },
//	{ "r",		cmd_run },
//	{ "run",	cmd_run },
//	{ "q\n",	cmd_quit },
//	{ "quit\n",	cmd_quit },
//        /* Please add more operations below. */
//        {NULL, NULL}
//};

/*
 * Process a single command.
 */
int cmd_dispatch(char *cmd)
{
	time_t beforesecs, aftersecs, secs;
	u_int32_t beforensecs, afternsecs, nsecs;
	char *args[MAXMENUARGS];
	int nargs=0;
	char *word;
	char *context;
 	int i, result;

	for (word = strtok_r(cmd, " ", &context);
	     word != NULL;
	     word = strtok_r(NULL, " ", &context)) {

		if (nargs >= MAXMENUARGS) {
			printf("Command line has too many words\n");
			return E2BIG;
		}
		args[nargs++] = word;
		otherargs[nargs - 1] = word;
	}

	if (nargs==0) {
		return 0;
	}

	for (i=0; cmdtable[i].name; i++) {
		if (*cmdtable[i].name && !strcmp(args[0], cmdtable[i].name)) {
			assert(cmdtable[i].func!=NULL);

            /*Qin: Call function through the cmd_table */
			result = cmdtable[i].func(nargs, args);
			return result;
		}
	}

	printf("%s: Command not found\n", args[0]);
	return EINVAL;
}

int run_interface() {
	policy = FCFS_ID; // Default initial policy to FCFS
	size_t parmsize = 64;
	char* parms = (char*) malloc(parmsize * sizeof(char));
	if (parms == NULL) {
		printf("run_interface() ...Error occured in malloc\n");
		exit(1);
	}

	printf("Welcome to William (Chris) White's batch schedular Version 0.1 alpha\n"
			"Type 'help' to find more about AUBatch commands.\n");
	while(1) {
		printf("> [? for menu]: ");
		getline(&parms, &parmsize, stdin);
		cmd_dispatch(parms);
	}
	return 0;
}
