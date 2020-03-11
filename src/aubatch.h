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
u_int run_head;
u_int circular; //This represents the last job buffer. Used when head goes back to 0
u_int count;
time_t timer;
time_t process_time;

/*
 * Variables used to track benchmarking
 */
u_int benchmark_running = 0;
u_int benchmark_started = 0;
u_int benchmark_start = 0;
u_int benchmark_end = 0;
u_int benchmark_curr_count = 0;
char* benchmark_name;
u_int benchmark_total_turnaround = 0;
u_int benchmark_total_waiting_time = 0;
u_int benchmmark_total_exec_time = 0;
u_int time_offset = 0;
time_t benchmark_total_time = 0;

/*
 * Variables to track all stats for displaying upon quit
 */
u_int total_turnaound = 0;
u_int total_waiting = 0;
u_int total_count = 0;
u_int total_exec = 0;

char *policyname;

/*
 * From Dr. Qin's aubatch_sample.c
 */
pthread_mutex_t cmd_queue_lock;  /* Lock for critical sections */
pthread_cond_t cmd_buf_not_full; /* Condition variable for buf_not_full */
pthread_cond_t cmd_buf_not_empty; /* Condition variable for buf_not_empty */





#endif /* AUBATCH_H_ */
