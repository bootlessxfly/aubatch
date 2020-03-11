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
#include "jobs.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int dispatch_jobs() {
	while (1) {
		pthread_mutex_lock(&cmd_queue_lock);
		while(count == 0) {
			pthread_cond_wait(&cmd_buf_not_empty, &cmd_queue_lock);
		}
		struct job j;
		// If the circular buffer just reset to the beginning,
		// use the job from the end of the buffer
		if (run_head == 0 && circular == 1) {
			j = jobs[JOB_QUEUE_SIZE - 1];
		}
		else {
			// else use the current head index
			j = jobs[run_head];
		}
		// Set the satus to running
		j.status = 1;
		jobs[run_head] = j;
		/*
		 * This indicates that we should start tracking a benchmarks results
		 */
		// Should look into adding benchmark_head back in
		if (benchmark_running == 1 && benchmark_started == 0) {
			benchmark_curr_count++;
			benchmark_started = 1;
			benchmark_total_time = time(NULL);
			time_offset = benchmark_total_time - timer;
		}
		else {
			benchmark_curr_count++;
		}
		run_head++;
		/*
		 * Reset the circular buffer
		 */
		if (run_head == JOB_QUEUE_SIZE) {
			run_head = 0;
		}
		pthread_cond_signal(&cmd_buf_not_empty);
		pthread_mutex_unlock(&cmd_queue_lock);
		/*
		 * The running of the process sits out side of the critical lock of
		 * the shared jobs queue so that while a job is processing, the CPU can still schedule other jobs
		 */
		run_job(j);
		if (benchmark_started == 1) {
			int now = time(NULL);
			int ending = now - benchmark_total_time;
			int turn = ending - (time_offset - j.arrivalTime);
			benchmark_total_turnaround = benchmark_total_turnaround + turn;
			benchmark_total_waiting_time = benchmark_total_waiting_time + j.waitingTime;
			benchmmark_total_exec_time = benchmmark_total_exec_time + j.exectime;
		}
		total_count++;
		pthread_mutex_lock(&cmd_queue_lock);
		jobs[run_head - 1].status = 2;
		pthread_mutex_unlock(&cmd_queue_lock);
		count--;
		/*
		 * This handles reporting benchmark info
		 */
		if (benchmark_curr_count == benchmark_end && benchmark_started == 1) {
			int now = time(NULL);
			int endingtime = now - benchmark_total_time;
			double turn =  ((double)benchmark_total_turnaround / (double) benchmark_end);
			double exect = ((double) benchmmark_total_exec_time / (double) benchmark_end);
			double waiting = ((double) benchmark_total_waiting_time / (double)benchmark_end);
			double through = ((double)benchmark_end / (double)endingtime);
			printf("The benchmark %s is over\n", benchmark_name);
			printf("Total number of jobs submitted: %d\n", benchmark_end);
			printf("Average turn around time: %f seconds\n", turn);
			printf("Average execution time: %f seconds\n", exect);
			printf("Average waiting time: %f seconds\n", waiting);
			printf("Throughput: %f\n", through);

			benchmark_curr_count = 0;
			benchmark_end = 0;
			benchmark_running = 0;
			benchmark_started = 0;
			benchmark_total_turnaround = 0;
			benchmmark_total_exec_time = 0;
			benchmark_total_waiting_time = 0;
			benchmark_total_time = 0;
			time_offset = 0;
		}

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
	int etime = j.exectime;
	args[0] = malloc(sizeof(int));
	sprintf(args[0], "%d", etime);
	args[1] = malloc(sizeof(int));
	sprintf(args[1], "%d", etime);
	args[2] = malloc(strlen(j._jobname));
	strcpy(args[2], j._jobname);
	args[3] = NULL;
	process_time = time(NULL);
	pid = fork();
	if (pid < 0) {
		puts("There was an error forking the process");
		return;
	}
	else if (pid == 0) {
		//todo remove target before submitting

		execv("./target/batch_job", args);
		//system("./target/batch_job 5 test");

	}
	else {
		wait();
	}
}


