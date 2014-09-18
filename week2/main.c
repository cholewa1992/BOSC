#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>

int main( int argc, char **argv) {
	printf("Calling the kernel\n");
	syscall(314);
}
