/*
 * jobs.h
 *
 *  Created on: Mar 8, 2020
 *      Author: chwhite
 */

#ifndef JOBS_H_
#define JOBS_H_

#include <stdlib.h>

struct job {
	char _jobname[100];
	int priority;
	double exectime;
	double arrivalTime;
	int status;

};

extern struct job jobs[10000];
extern u_int job_head;
extern u_int job_tail;
extern u_int count;
extern time_t timer;

#endif /* JOBS_H_ */
