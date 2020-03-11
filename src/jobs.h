/*
 * jobs.h
 *
 *  Created on: Mar 8, 2020
 *      Author: chwhite
 */

#ifndef JOBS_H_
#define JOBS_H_

#define DEBUG 0
#define JOB_QUEUE_SIZE 1000

#include <stdlib.h>

struct job {
	char _jobname[100];
	int priority;
	int exectime;
	int arrivalTime;
	int waitingTime;
	char *arrivalTimeString;
	int status;

};

extern struct job jobs[JOB_QUEUE_SIZE];
extern u_int job_head;
extern u_int run_head;
extern u_int circular;
extern u_int count;
extern time_t timer;

/*
 * Used to track individual benchmark run
 * Only one benchmark can run at a time
 */
extern u_int benchmark_running;
extern u_int benchmark_started;
extern u_int benchmark_start;
extern u_int benchmark_end;
extern u_int benchmark_curr_count;
extern char* benchmark_name;
extern u_int benchmark_total_turnaround;
extern u_int benchmark_total_waiting_time;
extern u_int benchmmark_total_exec_time;
extern u_int time_offset;
extern time_t benchmark_total_time; // Used for throughput

/*
 * Variables to track all stats for displaying upon quit
 */
extern u_int total_turnaound;
extern u_int total_waiting;
extern u_int total_count;


/*
 * The process time measures current progress of a process in seconds
 * It is used to accurately measure waiting time.
 */
extern time_t process_time;

extern char *policyname;

extern pthread_mutex_t cmd_queue_lock;  /* Lock for critical sections */
extern pthread_cond_t cmd_buf_not_full; /* Condition variable for buf_not_full */
extern pthread_cond_t cmd_buf_not_empty; /* Condition variable for buf_not_empty */

#endif /* JOBS_H_ */
