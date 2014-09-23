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

/* --- execute a shell command --- */
int executeshellcmd (Shellcmd *shellcmd)
{
	printshellcmd(shellcmd);
	
	Cmd *cmdlist = shellcmd -> the_cmds;
	
	// in and out
	char *in = shellcmd->rd_stdin;
	char *out = shellcmd->rd_stdout;

	while(cmdlist != NULL){
		char **cmd = cmdlist->cmd;
		cmdlist = cmdlist->next;	
		char **runcmd = cmd;	
		
		if(*runcmd != NULL){
			
			if( !isValidCmd(runcmd) );
				exit(0);

			pid_t pid = fork();
			if(pid == 0){
				
				if (in != NULL && out != NULL){ // redirect to input and ouput file
					redirInOut(in, out, runcmd);
				}else if(in != NULL){						// redirect to input file
					redirIn(in, runcmd);
				}else if(out != NULL) {					// redirect to output file
					redirOut(out, runcmd);
				}else{													// don't redirect
					execvp(*runcmd, runcmd);
					exit(0);
				}

			}else{
				int status = 0;	
				waitpid(pid,&status,0);
			}			
		}

	}
 	return 0;
}

int isValidCmd(char *runcmd){
	puts("validator");
	
	const char *path1 = "/bin/";
	const char *path2 = "/usr/bin/";

	if(access(path1, F_OK)){
		puts("Yipee");
		return 0;
	}
	return -1; // command not found
}


// either pipe the result of running the command into another process/program or run the command.
//int run(char *runcmd, char pipe){
//	if(pipe){
//		pipe exevp(*runcmd, runcmd);
//	}else{
//		exevp(*runcmd, runcmd);
//	}

//}


// redirect in and out
// Note why is a file format (e.g. '.txt') necessary?
int redirInOut(char *inFile, char *outFile, char **runcmd ){
	
	puts("IN-OUT");
  int fidIn  = open(inFile, O_RDONLY);
	int fidOut = open(outFile, O_WRONLY | O_CREAT | O_APPEND); // 				
	close(0); close(1);
	dup(fidIn); dup(fidOut);
	int status = execvp(*runcmd, runcmd);
	close(fidIn); close(fidOut);
	if(status != 0) 
		printf("execvp returned: %i, errno returned: %i 'no such file or directory' \n", status, errno);

}

// redirect in
int redirIn(char *inFile, char **runcmd){
	int fid = open(inFile, O_RDONLY);  
	close(0); // close standard input
	dup(fid);	// 'duplicate fileid', opens another input (file)	 
	int status = execvp(*runcmd, runcmd);
	close(fid);				
	if(status != 0) 
		printf("execvp returned: %i, errno returned: %i 'no such file or directory' \n", status, errno);


}

// redirect out
int redirOut(char *outFile, char **runcmd){
	int fid = open(outFile, O_WRONLY | O_CREAT | O_APPEND); // 				
	close(1); // close standard output
	dup(fid); // duplicate file-descriptor

	int status = execvp(*runcmd, runcmd);
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
