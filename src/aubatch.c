/*
 ============================================================================
 Name        : aubatch.c
 Author      : Christoph White
 Version     :
 Copyright   : 
 Description : Peices of this have come from Dr. Qin's aubatch_sample.c file
 This is the parent thread for the entire program. It hanldes initializing all of the
 variables in the shared memory realm, and creates the commandline/scheduling
 and dispatcher threads.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "commandline_parser.h"
#include "aubatch.h"

#include "dispatcher.h"


int main(void) {
	pthread_t cli_thread, dispatch_thread;
	char cli_message = "Running the CLI and job creation thread";
	char dispatch_message = "Running the dispatchor thread";

	int ret, ret1;
	job_head = 0;
	run_head = 0;
	circular = 0;
	count = 0;
	policyname = malloc(7);
	strcpy(policyname, "FCFS");
	timer = time(NULL);
	ret = pthread_create(&cli_thread, NULL, run_cli, (void*) cli_message);
	ret1 = pthread_create(&dispatch_thread, NULL, run_dispatchor, (void*) dispatch_message);

    pthread_mutex_init(&cmd_queue_lock, NULL);
    pthread_cond_init(&cmd_buf_not_full, NULL);
    pthread_cond_init(&cmd_buf_not_empty, NULL);

    pthread_join(cli_thread, NULL);
    pthread_join(dispatch_thread, NULL);

	return EXIT_SUCCESS;
}

int run_cli() {
	return run_interface();
}

int run_dispatchor() {
	return dispatch_jobs();
}
