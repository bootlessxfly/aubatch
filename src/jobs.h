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
	double exectime;
	double arrivalTime;
	int status;

};

extern struct job jobs[JOB_QUEUE_SIZE];
extern u_int job_head;
extern u_int job_tail;
extern u_int count;
extern time_t timer;

extern pthread_mutex_t cmd_queue_lock;  /* Lock for critical sections */
extern pthread_cond_t cmd_buf_not_full; /* Condition variable for buf_not_full */
extern pthread_cond_t cmd_buf_not_empty; /* Condition variable for buf_not_empty */

#endif /* JOBS_H_ */
