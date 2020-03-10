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
		/*
		 * The running of the process sits out side of the critical lock of
		 * the shared jobs queue so that while a job is processing, the CPU can still schedule other jobs
		 */
		run_job(j);
		count--;

		circular = 1;

//		if (circular == JOB_QUEUE_SIZE) {
//			circular = 0;
//		}
	}

}

/*
 * Kick off a process using execv
 * Some of this code was based on Dr. Qin's "fork_execv.c"
 */
void run_job(struct job j) {
	pid_t pid;
	char *args[5];
//	  args[0] = "5";
//	  args[1] = "5";
//	  args[2] = "test";
//	  args[3] = NULL;
	int time = j.exectime;
	args[0] = malloc(sizeof(int));
	sprintf(args[0], "%d", time);
	args[1] = malloc(sizeof(int));
	sprintf(args[1], "%d", time);
	args[2] = malloc(strlen(j._jobname));
	strcpy(args[2], j._jobname);
	args[3] = NULL;

	pid = fork();
	if (pid < 0) {
		puts("There was an error forking the process");
		return;
	}
	else if (pid == 0) {
		puts("Child running ...");
		//todo remove target before submitting

		execv("./target/batch_job", args);
		//system("./target/batch_job 5 test");

	}
	else {
		wait();
	}

//	//#ifdef DEBUG
//		printf("Running job: %s\n", j._jobname);
//	//#endif
//		// A call to execv should go here
//		int time = (int) j.exectime;
//				printf("Time to run is: %d\n", time);
//	sleep((int) time);
//	//#ifdef DEBUG
//		printf("Finished job: %s\n", j._jobname);
//	//#endif
}


