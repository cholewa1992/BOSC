/* 
   bosh.c : BOSC shell 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "parser.h"
#include "print.h"

#include "forback.h"
#include "redirect.h"
#include "pipe.h"


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
	
	if(cmdlist != NULL){
		char **cmd = cmdlist->cmd;
		cmdlist = cmdlist->next;
	
		char **runcmd = cmd;	
		if(*runcmd != NULL){
			pid_t pid = fork();
			if(pid == 0){
				execvp(*runcmd, runcmd);
				exit(0);
			}else{
				int status = 0;	
				waitpid(pid,&status,0);
			}			
		}

	}
 	return 0;
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
