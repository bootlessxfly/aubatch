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
	while (1) {
		pthread_mutex_lock(&cmd_queue_lock);
		while(count == 0) {
			pthread_cond_wait(&cmd_buf_not_empty, &cmd_queue_lock);
		}
		struct job j;
		// If the circular buffer just reset to the beginning,
		// use the job from the end of the buffer
		if (job_head == 0 && circular == 1) {
			j = jobs[JOB_QUEUE_SIZE - 1];
		}
		else {
			// else use the current head index
			j = jobs[job_head - 1];
		}

		pthread_cond_signal(&cmd_buf_not_empty);
		pthread_mutex_unlock(&cmd_queue_lock);
		run_job(j);
		count--;

		circular = 1;

//		if (circular == JOB_QUEUE_SIZE) {
//			circular = 0;
//		}
	}

}

void run_job(struct job j) {
	//#ifdef DEBUG
		printf("Running job: %s\n", j._jobname);
	//#endif
		// A call to execv should go here
		int time = (int) j.exectime;
				printf("Time to run is: %d\n", time);
	sleep((int) time);
	//#ifdef DEBUG
		printf("Finished job: %s\n", j._jobname);
	//#endif
}


