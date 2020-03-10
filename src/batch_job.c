
/* 
 * COMP7500/7506
 * Project 3: process.c 
 *
 * This program will be launched in fork_execv (see fork_execv.c).
 *
 * Compilation Instruction: 
 * gcc process.c -o process
 * 
 * Reference: 
 * https://faq.cprogramming.com/cgi-bin/smartfaq.cgi?answer=1044654269&id=1043284392
 * Modified by Xiao Qin
 * Department of Computer Science and Software Engineering
 * Auburn University
 * Feb. 6, 2018.
 */
#include <stdio.h> 

int main(int argc, char *argv[] )
{
  int time = atoi(argv[1]);
  printf("Running process %s ... \n", argv[2]);
  sleep(time);
  printf("Done ... \n");

  return 0;
}
