#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>

int calc(int r){
	struct timeval tp1, tp2;
	gettimeofday( &tp1, NULL );

	double x = 1;
	int n = 10000000;
	for(int i = 0; i < (n * (10-r)); i++){
		x = sin(x);
	}

	gettimeofday( &tp2, NULL );
	int time = (tp2.tv_sec-tp1.tv_sec)*1000+(tp2.tv_usec - tp2.tv_usec)/1000;
	printf("Process nr: %i finish in time (ms): %d\n",(r+1), time );
}

int make(int r){
	int status;
	pid_t pid = fork();
	if(pid == 0){
		calc(r);
	}
	else{
		if(++r < 10)
			make(r);
		waitpid(pid, &status, 0);
	}
}

int main(int argc, char** argv){
	struct sched_param param;
	sched_getparam( 0, &param );
	param.sched_priority = sched_get_priority_max( SCHED_FIFO ); //SCHED_FIFO
	if( sched_setscheduler( 0, SCHED_FIFO, &param) == -1){
		perror("change of scheduler failed");
		exit(-1);
	}
	make(1);
}


