/*
 ============================================================================
 Name        : scheduler.c
 Author      : Christoph White
 Version     :
 Copyright   :
 Description : This is partially based off of Dr. Qin's aubatch_sample example
 This hanldes taking in a job and adding it to the job queue based on a policy
 It handles creating and populating the job structure as well as calulating
 arrival and waiting time. If a policy change comes in, there is functionality
 to reschedule the queue based on the new policy.
 ============================================================================
 */
#include "schedular.h"
#include <string.h>
#include <time.h>
#include "shared_memory.h"

int set_policy(int policy) {
	if (policy == FCFS_ID) {
		strcpy(policyname, "FCFS");
	}
	else if (policy == SJF_ID) {
		strcpy(policyname, "SJF");
	}
	else if (policy == PRIO_ID) {
		strcpy(policyname, "priority");
	}

}

int add_job(int policy, char *name, char *etime, char* prio) {
	set_policy(policy);
	struct job j;
	create_job(name, etime, prio, &j);
	pthread_mutex_lock(&cmd_queue_lock);
	while (count == JOB_QUEUE_SIZE) {
		pthread_cond_wait(&cmd_buf_not_full, &cmd_queue_lock);
	}
	if (policy == FCFS_ID) {
		fcfs(j);
	}
	else if (policy == SJF_ID) {
		sjf(j);
	}
	else if (policy == PRIO_ID) {
		priority(j);
	}
	count++;
	job_head++;
	if (job_head == JOB_QUEUE_SIZE) {
		job_head = 0;
	}
	pthread_cond_signal(&cmd_buf_not_empty);
	pthread_mutex_unlock(&cmd_queue_lock);
	printf("Job %s was submitted.\n"
			"Total number of jobs in the queue: %d\n"
			"Expected waiting time is: %d seconds\n"
			"Scheduling Policy: %s\n",
			j._jobname, count, j.waitingTime, policyname);
}

int fcfs(struct job j) {
	/*
	 * Only set the arrival time once actually placed in queue.
	 */

	set_arrival(&j);

	jobs[job_head] = j;



	return EXIT_SUCCESS;

}

int sjf(struct job j) {
	set_arrival(&j);
	jobs[job_head] = j; //add to end of queue
	struct job temp;
	int pos;
	// Sort Current Queue
	for (int i = run_head; i < job_head; i++) {
		pos = i;
		for (int k = i + 1; k <= job_head; k++) {
			if (jobs[k].exectime < jobs[pos].exectime) {
				pos = k;
			}
		}
		temp = jobs[i];
		jobs[i] = jobs[pos];
		jobs[pos] = temp;
	}

}

int priority(struct job j) {
	set_arrival(&j);
	jobs[job_head] = j; //add to end of queue
	struct job temp;
	int countcheck = 0;
	int pos;
	// Sort Current Queue
	for (int i = run_head; i < job_head; i++) {
		pos = i;
		for (int k = i + 1; k <= job_head; k++) {
			if (jobs[k].priority < jobs[pos].priority) {
				pos = k;
			}
		}
		temp = jobs[i];
		jobs[i] = jobs[pos];
		jobs[pos] = temp;
	}

}

int reschedule_fcfs() {
	set_policy(FCFS_ID);
	if (count == 0) {
		return 0;
	}
	pthread_mutex_lock(&cmd_queue_lock);
	while (count == JOB_QUEUE_SIZE) {
		pthread_cond_wait(&cmd_buf_not_full, &cmd_queue_lock);
	}
	struct job temp;
	int pos;
	// Sort Current Queue
	for (int i = run_head; i < job_head; i++) {
		pos = i;
		for (int k = i + 1; k < job_head; k++) {
			if (jobs[k].arrivalTime < jobs[pos].arrivalTime) {
				pos = k;
			}
		}
		temp = jobs[i];
		jobs[i] = jobs[pos];
		jobs[pos] = temp;
	}
	pthread_cond_signal(&cmd_buf_not_empty);
	pthread_mutex_unlock(&cmd_queue_lock);
}

int reschedule_sjf() {
	set_policy(SJF_ID);
	if (count == 0) {
		return 0;
	}
	pthread_mutex_lock(&cmd_queue_lock);
	while (count == JOB_QUEUE_SIZE) {
		pthread_cond_wait(&cmd_buf_not_full, &cmd_queue_lock);
	}
	struct job temp;
	int pos;
	// Sort Current Queue
	for (int i = run_head; i < job_head; i++) {
		pos = i;
		for (int k = i + 1; k < job_head; k++) {
			if (jobs[k].exectime < jobs[pos].exectime) {
				pos = k;
			}
		}
		temp = jobs[i];
		jobs[i] = jobs[pos];
		jobs[pos] = temp;
	}
	pthread_cond_signal(&cmd_buf_not_empty);
	pthread_mutex_unlock(&cmd_queue_lock);
}

int reschedule_priority() {
	set_policy(PRIO_ID);
	if (count == 0) {
		return 0;
	}
	pthread_mutex_lock(&cmd_queue_lock);
	while (count == JOB_QUEUE_SIZE) {
		pthread_cond_wait(&cmd_buf_not_full, &cmd_queue_lock);
	}
	struct job temp;
	int pos;
	// Sort Current Queue
	for (int i = run_head; i < job_head; i++) {
		pos = i;
		for (int k = i + 1; k < job_head; k++) {
			if (jobs[k].priority < jobs[pos].priority) {
				pos = k;
			}
		}
		temp = jobs[i];
		jobs[i] = jobs[pos];
		jobs[pos] = temp;
	}
	pthread_cond_signal(&cmd_buf_not_empty);
	pthread_mutex_unlock(&cmd_queue_lock);
}

/*
 *
 */
int calc_wait() {
	sleep(1); //Sleep for one second to simulate arrival time
	int start;
	int end;
	int totalwait = 0;
	if (count == 0) {
		return 0;
	}
	if (run_head == 0) {
		start = run_head;
	}
	else {
		start = run_head - 1;
	}
	end = start + count;
	for (int i = start; i < end; i++) {
		if (jobs[i].status == 1) {
			int now = time(NULL);
			int elapsed = now - process_time;
			totalwait = totalwait + (jobs[i].exectime - elapsed);
		}
		else {
			totalwait = totalwait + jobs[i].exectime;
		}
		if (totalwait < 0) {
			// An error occur, we need to recalculate wiating time
			i = start;
			totalwait = 0;
		}
		if (totalwait == 0) {
			// Some error occured, lets recalc.
			i = start;
			totalwait = 0;
		}
	}

	return totalwait;
}

/*
 * This stores the arrival time in seconds
 * And then stores the arivval time as HH:MM:SS for the list functionl ater on
 */
void set_arrival(struct job *j) {
	time_t secs = time(NULL);
	int time = ((secs - timer));
	j->arrivalTime = time;
	build_string(time, j);


}

/*
 * Builds the arrival time string for printing
 */
void build_string(int time, struct job *j) {
	char *arrival = malloc(8);
	char *buff = malloc(3);
	int hour = 0;
	int r = 0;
	int minute = 0;
	int second = 0;
	if (time >= 360) {
		// remaining seconds after we get hours
		r = time % 360;
		hour = time / 360;
		if (r >= 60) {
			second = r % 60;
			minute = r / 60;
		}
		else {
			second = r;
		}
	}
	else if (time >= 60) {
		second = time % 60;
		minute = time / 60;
	}
	else {
		second = time;
	}

	if (hour > 0) {
		sprintf(buff, "%d", hour);
		if (hour >= 10) {
			strcpy(arrival, buff);
		}
		else {
			strcpy(arrival, "0");
			strcat(arrival, buff);
		}
		strcat(arrival,":");
		sprintf(buff, "%d", minute);
		if(minute >= 10) {
			strcat(arrival, buff);
		}
		else {
			strcat(arrival, "0");
			strcat(arrival,buff);
		}
		strcat(arrival,":");
		sprintf(buff, "%d", second);
		if (second >= 10) {
			strcat(arrival, buff);
		}
		else {
			strcat(arrival, "0");
			strcat(arrival,buff);
		}
	}
	else if (minute > 0) {
		strcpy(arrival, "00:");
		sprintf(buff, "%d", minute);
		if(minute >= 10) {
			strcat(arrival, buff);
		}
		else {
			strcat(arrival, "0");
			strcat(arrival,buff);
		}
		strcat(arrival,":");
		sprintf(buff, "%d", second);
		if (second >= 10) {
			strcat(arrival, buff);
		}
		else {
			strcat(arrival, "0");
			strcat(arrival,buff);
		}
	}
	else {
		strcpy(arrival, "00:00:");
		sprintf(buff, "%d", second);
		if (second >= 10) {
			strcat(arrival, buff);
		}
		else {
			strcat(arrival, "0");
			strcat(arrival,buff);
		}
	}

	arrival[strlen(arrival)] = '\0';
	j->arrivalTimeString = malloc(strlen(arrival));
	strcpy(j->arrivalTimeString, arrival);
}




void create_job(char *name, char *etime, char *prio, struct job *j) {
	strcpy(j->_jobname, name);
	j->_jobname[strlen(j->_jobname)] = '\0';
	j->exectime = atoi(etime);
	j->priority = atoi(prio);
	j->waitingTime = calc_wait();
}

