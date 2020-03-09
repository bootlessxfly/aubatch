/*
 * aubatch.h
 *
 *  Created on: Mar 8, 2020
 *      Author: chwhite
 */

#ifndef AUBATCH_H_
#define AUBATCH_H_
#include "jobs.h"

int run_cli();
int run_dispatchor();

struct job jobs[JOB_QUEUE_SIZE];
u_int job_head;
u_int job_tail; // The end of the queue. job_tail - 1 is the queue ending index.
u_int count;
time_t timer;

/*
 * From Dr. Qin's aubatch_sample.c
 */
pthread_mutex_t cmd_queue_lock;  /* Lock for critical sections */
pthread_cond_t cmd_buf_not_full; /* Condition variable for buf_not_full */
pthread_cond_t cmd_buf_not_empty; /* Condition variable for buf_not_empty */





#endif /* AUBATCH_H_ */
