/*
 * schedular.c
 *
 *  Created on: Mar 8, 2020
 *      Author: chwhite
 */
#include "schedular.h"
#include "jobs.h"
#include <string.h>

int run_policy(int policy, char *otherargs[]) {
	/*
	 * Create the job
	 */
	struct job j;
	strcpy(j._jobname, otherargs[1]);
	j._jobname[strlen(j._jobname)] = '\0';
	j.exectime = (double) atoi(otherargs[2]);
	j.priority = atoi(otherargs[3]);
	//create_job(otherargs);
	//create_job(otherargs, j);
	if (policy == FCFS_ID) {
		fcfs(j);
	}
}

int fcfs(struct job j) {
	puts("Adding new job under FCFS");
	/*
	 * Only set the arrival time once actually placed in queue.
	 */
	j.arrivalTime = timer;
	jobs[job_tail] = j;
	job_tail++;

}

struct job create_job(char *otherargs[]) {
	struct job j;
	strcpy(j._jobname, otherargs[1]);
	j._jobname[strlen(j._jobname)] = '\0';
	j.exectime = (double) atoi(otherargs[2]);
	j.priority = atoi(otherargs[3]);
	return j;
}

// Allows us to add the arrival time right before we add it to the queue
struct job add_arrival(struct job j) {
	j.arrivalTime = timer;
}
