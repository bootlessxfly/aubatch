/*
 ============================================================================
 Name        : jobs.h
 Author      : Christoph White
 Version     :
 Copyright   :
 Description : This is a shared memory realm to be used between threads. All
 modules in the program have access to it. This contains the job structure
 definiation as well as the job queue. It also contains the control variables
 and locks to control access to the queue. In addition, this contains many other
 variables used for time keeping, metric gathering, and benchmarking.
 ============================================================================
 */

#ifndef SHARED_MEMORY_H_
#define SHARED_MEMORY_H_

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

/*
 * Variables related to the job queue
 */
extern struct job jobs[JOB_QUEUE_SIZE];
extern u_int job_head;
extern u_int run_head;
extern u_int circular;
extern u_int count;


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
extern u_int total_exec;
extern time_t timer;


/*
 * The process time measures current progress of a process in seconds
 * It is used to accurately measure waiting time.
 */
extern time_t process_time;

extern char *policyname;

extern pthread_mutex_t cmd_queue_lock;  /* Lock for critical sections */
extern pthread_cond_t cmd_buf_not_full; /* Condition variable for buf_not_full */
extern pthread_cond_t cmd_buf_not_empty; /* Condition variable for buf_not_empty */

#endif /* SHARED_MEMORY_H_ */
