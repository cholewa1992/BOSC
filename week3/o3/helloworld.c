#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
	printf("I'm a process that just started!\n");
	for(int i = 0; i < 3; i++){
		printf("I'm a process!\n");
		sleep(1);
	}
	printf("I'm quitting now!\n");
	exit(0);
}
