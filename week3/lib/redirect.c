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
int redirect_stdincmd(char *filename, char *argv[], char *infilename)
{
  return 0;
}

/* start the program specified by filename with the arguments in argv 
   in a new process that has its stdout redirected to outfilename and 
   wait for termination */
int redirect_stdoutcmd(char *filename, char *argv[], char *outfilename)
{
  return 0;
}
