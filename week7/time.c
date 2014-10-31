#include <sys/time.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <math.h>
#include <sched.h>


int main(int argc, char** argv){
	struct sched_param param;
	sched_getparam( 0, &param );
	param.sched_priority = sched_get_priority_max( SCHED_FIFO );
	if( sched_setscheduler( 0, SCHED_FIFO, &param) == -1){
		perror("change of scheduler failed");
		exit(-1);
	}
	calc(0);
}

int calc(int r){
	pid_t pid = fork();
	int status;
	struct timeval tp1, tp2;
	if(pid == 0){
		gettimeofday( &tp1, NULL );
		double x = 1;
		int n = 1000000;

		for(int i = 0; i < n; i++){
			x = sin(x);
		}	

		gettimeofday( &tp2, NULL );
		int time = (tp2.tv_sec-tp1.tv_sec)*1000+(tp2.tv_usec - tp2.tv_usec)/1000;
		printf("time (ms): %d\n", time );
	}else{
		if(r < 10){
			waitpid(pid, &status, 0);
			calc(++r);
		}
	}
}

