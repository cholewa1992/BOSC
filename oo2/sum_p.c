#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <math.h>

#define MAX_NUMBER_OF_THREADS 32

double sum[MAX_NUMBER_OF_THREADS];
void *runner(void *param);
typedef struct _runnerargs {
	int start;
	int end;
	double *sum;
} Runnerargs;

int main(int argc, char *argv[]){
	pthread_t tid[MAX_NUMBER_OF_THREADS];
	
	if(argc != 3){
		fprintf(stderr, "usage: ./sum_p <integer value> <integer value>\n");
		return -1;
	}
	if(atoi(argv[1]) < 0){
		fprintf(stderr, "%d must be >= 0\n", atoi(argv[1]));
		return -1;
	}
	if(atoi(argv[2]) < 0){
		fprintf(stderr, "%d must be >= 0\n", atoi(argv[2]));
		return -1;
	}
	if(atoi(argv[2]) > MAX_NUMBER_OF_THREADS){
		fprintf(stderr, "Max number of threads is 16!\n");
		return -1;
	}
	

	int sumto = atoi(argv[1]);
	int nthreads = atoi(argv[2]);
	Runnerargs args[nthreads];	
	
	for(int i = 0, cut, last = 0, threadsleft = nthreads; i < nthreads; i++, sumto -= cut, threadsleft--, last+=cut){
		cut = sumto / threadsleft; //Calculating the range this process should calculate
		args[i] = (Runnerargs)  {(last + 1),(last + cut),&sum[i]}; //Preparing the arguments for the process
		pthread_create(&tid[i],NULL,runner,&args[i]); //Starting the proces
	}

	double total_sum = 0;
	for(int i = 0; i < nthreads; i++){
		pthread_join(tid[i],NULL);
		total_sum += sum[i];			
	}

	printf("sum = %f\n", total_sum);
}

void *runner(void *args){
	Runnerargs *rargs = args;

	double sum = 0.0;
	for(int i = rargs->start; i <= rargs->end; i++){
		sum += sqrt(i);
	}

	*rargs->sum = sum;
	pthread_exit(0);
}
	

