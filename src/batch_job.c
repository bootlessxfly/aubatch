/*
 ============================================================================
 Name        : batch_job.c
 Author      : Christoph White
 Version     :
 Copyright   :
 Description : This was loosely based off of Dr. Qin's process.c example. This
 program is simple. It simulates a running process. The program takes in two parameters:
 The job name and the job run time. It simulates a job by sleeping the length of exec time.
 ============================================================================
 */
#include <stdio.h> 

int main(int argc, char *argv[] )
{
  int time = atoi(argv[1]);
  printf("Running process %s for %d... \n", argv[2], time);
  sleep(time);
  printf("Done ... \n");

  return 0;
}
