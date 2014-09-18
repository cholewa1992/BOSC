/* 
   Opgave 3 - pipe.c
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "pipe.h"

int main (int argc, char **argv){
	char *argv1[0];
	char *argv2[0];
	pipecmd("./helloworld", argv1, "./print", argv2);
}
/* create a pipe between two new processes, executing the programs 
   specified by filename1 and filename2 with the arguments in argv1
   and argv2 and wait for termination */
int pipecmd(char *filename, char *argv[], char *filename2, char *argv2[])
{
	int status = 0;
	int fd[2];
	if (pipe(fd) < 0){
		printf("Error while creating pipe... Terminating program");
		exit(1);
	}
	
	pid_t pid = fork();
	if(pid == 0){
		printf("Child process will now start %s and redirect stdout to pipe out\n", filename);
		close(fd[0]); //closing read pipe
		close(1); //closing stdout
		dup(fd[1]); //duplicating fd[1] to stdout
		close(fd[1]); //Closing the old out pipe
		execvp(filename,argv);
	}else{
		printf("Parrent process will now start %s and redirect pipe in to stdin\n", filename2);
		close(fd[1]); //Closing the out pipe
		close(0); //Closing stdin
		dup(fd[0]); //Duplicating fd[0] to stdin
		close(fd[0]); //Closing the old in pipe
		execvp(filename2,argv2);
		waitpid(pid, &status, 0);
	}
  	return 0;
}
