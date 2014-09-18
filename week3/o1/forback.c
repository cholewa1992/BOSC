#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "forback.h"


int main (int argc, char *argv[]){
	foregroundcmd("./helloworld",argv);
}



/* start the program specified by filename with the arguments in argv 
   in a new process and wait for termination */
int foregroundcmd(char *filename, char *argv[])
{
	int status;
	pid_t pid = fork();
	if(pid == 0){
		printf("now executing file in child process\n");
		execvp(filename, argv);
	} else {
		printf("waiting for child process\n");
		waitpid(pid, &status, 0);
		printf("now terminating parent!\n");
	}
  return 0;
}

/* start the program specified by filename with the arguments in argv 
   in a new process */
int backgroundcmd(char *filename, char *argv[])
{
	int status;
	pid_t pid = fork();
	if(pid == 0){
		printf("now executing file in child process\n");
		execvp(filename, argv);
	} else {
		printf("now terminating parent!\n");
	}
  return 0;
}
