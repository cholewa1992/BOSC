#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
	int c;
	while((c = getchar()) != EOF) putchar(c);
	exit(0);
}
