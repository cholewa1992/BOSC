/* 
   bosh.c : BOSC shell 
*/

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "parser.h"
#include "print.h"
#include "fcntl.h"
#include <errno.h>

/* --- symbolic constants --- */
#define HOSTNAMEMAX 100

/* --- use the /proc filesystem to obtain the hostname --- */
char *findhostname(char *hostname)
{
	gethostname(hostname,HOSTNAMEMAX);
	return hostname;
}


int executeshellcmd(Shellcmd *shellcmd){
	int status = 0;
	Cmd *cmdlist = shellcmd -> the_cmds;

	while( cmdlist != NULL){
		char **cmd = cmdlist -> cmd;
		cmdlist = cmdlist -> next;
		status = isValidCmd(cmd);

		if(status == 0){ 
			printf("%s: command not found\n", *cmd );
			return 0;
		}
		if(status == -1) return 1;
	}
	
	status = 0;
	cmdlist = shellcmd -> the_cmds;

	pid_t pid = fork();
	if(pid == 0){
		char *in = shellcmd -> rd_stdin;
		char *out = shellcmd -> rd_stdout;
		if(cmdlist != NULL){
			if(in == NULL && out == NULL){
				status = executecmd(cmdlist);
			}else{
				if(in != NULL && out != NULL) status = redirInOut(in, out, cmdlist);
				if(in != NULL) status = redirIn(in, cmdlist);
				if(out != NULL) status = redirOut(out, cmdlist);
			}
			return 0;
		}	
	}else{
		if(shellcmd -> background == 0){
			int wstatus = 0;	
			waitpid(pid,&wstatus,0);
		}
	}
	return 0;
}

int executecmd(Cmd *cmdlist){
	int status;
	char **cmd = cmdlist -> cmd;

	Cmd *next = cmdlist -> next;
	if(next != NULL){

		int fd[2];
		pipe(fd);

		pid_t pid = fork();
		if(pid == 0){
			close(fd[0]);
			close(1);
			dup(fd[1]);
			close(fd[1]);
			status = executecmd(next);
		}else{
			close(fd[1]);
			close(0);
			dup(fd[0]);
			close(fd[0]);
			status = execvp(*cmd,cmd);
			
			int wstatus;
			waitpid(pid,&wstatus,0);
		}
	}else{
		status = execvp(*cmd,cmd);
	}
	return status;
}



int isValidCmd(char **cmd){
	if(strncmp(*cmd,"exit",4) == 0) return -1;
	char str1[100];
	char str2[100];

	char *path1 = "/bin/";
	char *path2 = "/usr/bin/";

	strcpy(str1,path1);
	strcat(str1, *cmd);
	strcpy(str2,path2);
	strcat(str2, *cmd);

	return access( str1, F_OK) != -1 || access (str2,F_OK) != -1;
}


// redirect in and out
// Note why is a file format (e.g. '.txt') necessary?
int redirInOut(char *inFile, char *outFile, Cmd *cmdlist){
	
  	int fidIn  = open(inFile, O_RDONLY);
	int fidOut = open(outFile, O_WRONLY | O_CREAT | O_APPEND); // 				
	close(0); close(1);
	dup(fidIn); dup(fidOut);
	int status = executecmd(cmdlist);
	close(fidIn); close(fidOut);
	if(status != 0) 
		printf("execvp returned: %i, errno returned: %i 'no such file or directory' \n", status, errno);

}

// redirect in
int redirIn(char *inFile, Cmd *cmdlist){
	int fid = open(inFile, O_RDONLY);  
	close(0); // close standard input
	dup(fid);	// 'duplicate fileid', opens another input (file)	 
	int status = executecmd(cmdlist);
	close(fid);				
	if(status != 0) 
		printf("execvp returned: %i, errno returned: %i 'no such file or directory' \n", status, errno);


}

// redirect out
int redirOut(char *outFile, Cmd *cmdlist){
	int fid = open(outFile, O_WRONLY | O_CREAT | O_APPEND); // 				
	close(1); // close standard output
	dup(fid); // duplicate file-descriptor

	int status = executecmd(cmdlist);
	close(fid);				
	if(status != 0) 
		printf("execvp returned: %i, errno returned: %i 'no such file or directory' \n", status, errno);

}

/* --- main loop of the simple shell --- */
int main (int argc, char *argv[]){
	/* initialize the shell */
	char *cmdline;
	char hostname[HOSTNAMEMAX];
	int terminate = 0;
	Shellcmd shellcmd;
  
  	if (findhostname(hostname)) {
    		/* parse commands until exit or ctrl-c */
    		while (!terminate) {
      			printf("%s", hostname);
      			if (cmdline = readline(":# ")) {
				if(*cmdline) {
	  				add_history(cmdline);
	  				if (parsecommand(cmdline, &shellcmd)) {
	    					terminate = executeshellcmd(&shellcmd);
	 				}
				}
				free(cmdline);
    			 } else terminate = 1;
   		 }
   		 printf("Exiting bosh.\n");
  	}    
  	return EXIT_SUCCESS;
}
