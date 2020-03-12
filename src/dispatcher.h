/*
 ============================================================================
 Name        : dispatcher.h
 Author      : Christoph White
 Version     :
 Copyright   :
 Description : This is partially based off of Dr. Qin's aubatch_sample example
 This handles the running each job that is in queue. This will not try to run
 any jobs unless the queue is not empty. This also handles keeping up with
 performance and metrics information
 ============================================================================
 */

#ifndef DISPATCHER_H_
#define DISPATCHER_H_

int dispatch_jobs();

char *proc_buffer[10];


#endif /* DISPATCHER_H_ */
