/* 

   Opgave 2

   redirect.c

 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "redirect.h"

/* start the program specified by filename with the arguments in argv 
   in a new process that has its stdin redirected to infilename and
   wait for termination */

int main(int argc, char *argv[]){
	redirect_stdoutcmd("./helloworld",argv, "out.log");
	redirect_stdincmd("./print",argv, "out.log");
	return 0;
}

int redirect_stdincmd(char *filename, char *argv[], char *infilename)
{
	int status;
        pid_t pid = fork();
        if(pid == 0){
                /* redirecting input */ 
		int fid = open(infilename, O_RDONLY);
		close(0);
		dup(fid);
		close(fid);

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
   in a new process that has its stdout redirected to outfilename and 
   wait for termination */
int redirect_stdoutcmd(char *filename, char *argv[], char *outfilename)
{
	int status;
        pid_t pid = fork();
        if(pid == 0){
                /* redirecting input */ 
		int fid = open(outfilename, O_CREAT | O_RDWR | S_IWUSR);
		close(1);
		dup(fid);
		close(fid);

		printf("now executing file in child process\n");
                execvp(filename, argv);
       	} else {
                printf("waiting for child process\n");
                waitpid(pid, &status, 0);
                printf("now terminating parent!\n");
	}
  	return 0;
}
