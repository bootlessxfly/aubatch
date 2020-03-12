/*
 ============================================================================
 Name        : commandline_parser.c
 Author      : Christoph White
 Version     :
 Copyright   :
 Description  This is the commandline interface for aubatch. I used Dr. Qin's commandline_parser.c
 as a reference to write this. When this program exits with the quit command, the
 parent process that created this thread will kill all of it's children thread except
 the currently running job.
 ============================================================================
 */

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "commandline_parser.h"
#include "schedular.h"
#include "string.h"
#include "shared_memory.h"


int cmd_bench(int nargs, char **args) {
	if (nargs != 7) {
		printf("usage: test <benchmark> <policy> <num_of_jobs> <priority_levels> <min_CPU_time> <max_CPU_time>\n");
		return EINVAL;
	}
	if (benchmark_running == 1) {
		printf("There is a benchmark currently running, please wait for it to finish\n");
		return EXIT_SUCCESS;
	}
	char* name = otherargs[1];
	benchmark_name = malloc(strlen(name));
	strcpy(benchmark_name, name);
	char* jname = malloc(3 * strlen(name));
	int policyid;
	int njob = atoi(otherargs[3]);
	int nprio = atoi(otherargs[4]) - 1; //Range is 0 to priority_levels -1
	int mintime = atoi(otherargs[5]);
	int maxtime = atoi(otherargs[6]);
	char* stime = malloc(strlen(otherargs[6]));
	char* sprio = malloc(strlen(otherargs[4]));
	char* jnum = malloc(strlen(otherargs[3]));
	int time = 0;
	int prio = 0;
	if (strcmp(otherargs[2], "FCFS") == 0 || strcmp(otherargs[2], "fcfs") == 0) {
		policyid = FCFS_ID;
	}
	else if (strcmp(otherargs[2], "SJF") == 0 || strcmp(otherargs[2], "sjf") == 0) {
		policyid = SJF_ID;
	}
	else if (strcmp(otherargs[2], "priority") == 0) {
		policyid = PRIO_ID;
	}
	else {
		printf("Provided policy %s is not valid. Refer to below list for valid options \n"
				"\tFCFS, SJF, policy.\n", otherargs[2]);
		return EXIT_FAILURE;
	}
	/*
	 * If the current polic does not match the benchmarks policy, switch it.
	 * If there are already jobs loaded, reschedule them
	 */
	if (policyid != policy && count > 0) {
		policy = policyid;
		if (policy == FCFS_ID) {
			reschedule_fcfs();
		}
		else if (policy == SJF_ID) {
			reschedule_sjf();
		}
		else if (policy == PRIO_ID) {
			reschedule_priority();
		}
	}
	else {
		policy = policyid;
	}
	benchmark_running = 1;
	if (count == 0) {
		benchmark_start = 0;
	}
	else {
		benchmark_start = job_head;
	}
	benchmark_end = njob;
	for (int i = 0; i < njob; i++) {
		prio = rand_range_gen(0, nprio);
		time = rand_range_gen(mintime, maxtime);
		strcpy(jname, name);
		sprintf(jnum, "%d", i + 1);
		strcat(jname, jnum);
		sprintf(stime, "%d", time);
		sprintf(sprio, "%d", prio);
		add_job(policyid, jname, stime, sprio);
	}
}

/*
 * This is used to generate priority and execution time based off
 * the range that the benchmark tool gives
 */
int rand_range_gen(int low, int high) {
	return (rand() % (high - low + 1)) + low;

}

int switch_priority(int nargs, char **args) {
	reschedule_priority();
}

int switch_fcfs(int nargs, char **args) {
	reschedule_fcfs();
}

int switch_sjf(int nargs, char **args) {
	reschedule_sjf();
}

int cmd_list(int nargs, char **args) {
	int start;
	int end;
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
	add_job(policy, otherargs[1], otherargs[2], otherargs[3]);
	struct job j = jobs[job_head];
    return 0; /* if succeed */
}

/*
 * The quit command.
 * Taken from Dr. Qin's "commlandline_parser.c" example
 */
int cmd_quit(int nargs, char **args) {
	//printf("Please display performance information before exiting AUbatch!\n");
	int now = time(NULL);
	int endingtime = now - timer;
	double turn =  ((double)total_turnaound / (double) total_count);
	double exect = ((double) total_exec / (double) total_count);
	double waiting = ((double) total_waiting / (double)total_count);
	double through = ((double)total_count / (double)endingtime);
	printf("The program is terminating. Here are some stats about the jobs that have been processed\n");
	if (total_count == 0) {
		printf("No jobs have been processed. Nothing to display ... \n");
		exit(0);
	}
	printf("Jobs have been processed. Below are the stats of all jobs since the program started ...\n");
	printf("Total number of jobs submitted: %d\n", total_count);
	printf("Average turn around time: %f seconds\n", turn);
	printf("Average execution time: %f seconds\n", exect);
	printf("Average waiting time: %f seconds\n", waiting);
	printf("Throughput: %f\n", through);
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

int cmd_helpmenu(int n, char **a)
{
	(void)n;
	(void)a;

	showmenu("AUbatch help menu", helpmenu);
	return 0;
}

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
	srand(time(0));
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
