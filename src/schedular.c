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
	pthread_mutex_lock(&cmd_queue_lock);
	while (count == JOB_QUEUE_SIZE) {
		pthread_cond_wait(&cmd_buf_not_full, &cmd_queue_lock);
	}
	j.arrivalTime = timer;
	jobs[count] = j;
	count++;

	job_head++;
	if (job_head == JOB_QUEUE_SIZE) {
		job_head = 0;
	}
	pthread_cond_signal(&cmd_buf_not_empty);
	pthread_mutex_unlock(&cmd_queue_lock);

}



//struct job create_job(char *otherargs[]) {
//	struct job j;
//	strcpy(j._jobname, otherargs[1]);
//	j._jobname[strlen(j._jobname)] = '\0';
//	j.exectime = (double) atoi(otherargs[2]);
//	j.priority = atoi(otherargs[3]);
//	return j;
//}
//
//// Allows us to add the arrival time right before we add it to the queue
//struct job add_arrival(struct job j) {
//	j.arrivalTime = timer;
//}
