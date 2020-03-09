/*
 ============================================================================
 Name        : AUBatch.c
 Author      : Christoph White
 Version     :
 Copyright   :
 Description : Used Dr. Qin's "process.c" Code as an example
 ============================================================================
 */

#include "dispatcher.h"

#include <stdio.h>
#include "jobs.h"


int dispatch_jobs() {
	printf("Dispatchor dispatching things now ...\n");
	pthread_mutex_lock(&cmd_queue_lock);
	while(count == 0) {
		pthread_cond_wait(&cmd_buf_not_empty, &cmd_queue_lock);
	}
	printf("Detected Job\n");
	struct job j = jobs[job_head];
	pthread_cond_signal(&cmd_buf_not_empty);
	pthread_mutex_unlock(&cmd_queue_lock);
	//run_job(j);
	puts("Ran");
}

void run_job(struct job j) {
	//#ifdef DEBUG
		printf("Running job: %s", j._jobname);
	//#endif
		// A call to execv should go here
	sleep(j.exectime);
	//#ifdef DEBUG
		printf("Finished job: %s", j._jobname);
	//#endif
}


