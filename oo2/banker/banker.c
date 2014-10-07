#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>
#include <pthread.h>

typedef struct state {
	int *resource;
	int *available;
	int **max;
	int **allocation;
	int **need;
} State;

// Global variables
int *work;
int *finish;
int m, n;
State *s = NULL;

// Mutex for access to state.
pthread_mutex_t state_mutex;

/* Random sleep function */
void Sleep(float wait_time_ms)
{
	// add randomness
	wait_time_ms = ((float)rand())*wait_time_ms / (float)RAND_MAX;
	usleep((int) (wait_time_ms * 1e3f)); // convert from ms to us
}

/* checking of arr1 is <= that arr */
int slarger(int arr1[], int arr2[], int n){
	for(int i = 0; i < n; i++){
		if(arr1[i] > arr2[i]) return 0;
	}
	return 1;
}

/* adding arr2 to arr1 */
void addarr(int arr1[], int arr2[], int n){
	for(int i = 0; i < n; i++){
		arr1[i] = arr1[i] + arr2[i];
	}
}

/* substrating arr2 from arr1 */
void subarr(int arr1[], int arr2[], int n){
	for(int i = 0; i < n; i++){
		arr1[i] = arr1[i] - arr2[i];
	}
}

/* Bankers safty alog */
int safety_check(){
	
	/* Set work = available */
	for(int i = 0; i < n; i++){
		work[i] = s->available[i];
	}
	
	/* set Finish[i] = false for all i */
	for(int i = 0; i < m; i++){
		finish[i] = 0;
	}
	
	
	/* If finish[i] == false and need[i] <= work then allocate, set finish[i] to true and restart loop */
	for(int i = 0; i < m; i++){
		if(finish[i] == 0 && slarger(s->need[i], work, n)){
			addarr(work, s->allocation[i], n);
			finish[i] = 1;
			i = 0;
		}
	}
	
	/* if any element in finish == false return 0, else return 1  */
	for(int i = 0; i < m; i++){
		if(finish[i] == 0) return 0;
	}	
	return 1;
}

/* Allocate resources in request for process i, only if it 
   results in a safe state and return 1, else return 0 */
int resource_request(int i, int *request)
{
	//Checking if the request exceeds the max need
	if(!slarger(request, s->need[i], n)){
		printf("ERROR!\n");
		exit(1);
	}

	/* Aquire lock for state */
	pthread_mutex_lock(&state_mutex);	
	
	/* if request > avalible then wait and call self recursively */
	if(!slarger(request, s->available, n)){
		pthread_mutex_unlock(&state_mutex);
		printf("r not a for p%i/n", i);
		Sleep(100);
		return resource_request(i, request);
	}
	
	/* Allocate resources */
	subarr(s->available, request, n);
	addarr(s->allocation[i], request, n);
	subarr(s->need[i], request, n);
	
	/* Check if safe state else reverse allocation, wait and call self recursively */
	if(!safety_check()){
		addarr(s->available, request, n);
		subarr(s->allocation[i], request, n);
		addarr(s->need[i], request, n);
		pthread_mutex_unlock(&state_mutex);
		Sleep(100);
		return resource_request(i, request);
	}

	/* Release lock for state */
	pthread_mutex_unlock(&state_mutex);
	return 1;
}

/* Release the resources in request for process i */
void resource_release(int i, int *request)
{
	/* Aquire lock for state */
	pthread_mutex_lock(&state_mutex);

	/* Release resources */
	addarr(s->available, request, n);
	subarr(s->allocation[i], request, n);
	addarr(s->need[i], request, n);

	/* Release lock for state */
	pthread_mutex_unlock(&state_mutex);
}

/* Generate a request vector */
void generate_request(int i, int *request)
{
	int j, sum = 0;
	while (!sum) {
		for (j = 0;j < n; j++) {
			request[j] = s->need[i][j] * ((double)rand())/ (double)RAND_MAX;
			sum += request[j];
		}
	}
	printf("Process %d: Requesting resources.\n",i);
}

/* Generate a release vector */
void generate_release(int i, int *request)
{
	int j, sum = 0;
	while (!sum) {
		for (j = 0;j < n; j++) {
			request[j] = s->allocation[i][j]; //* ((double)rand())/ (double)RAND_MAX;
			sum += request[j];
		}
	}
	printf("Process %d: Releasing resources.\n",i);
}

/* Threads starts here */
void *process_thread(void *param)
{
	/* Process number */
	int i = (int) (long) param, j;
	/* Allocate request vector */
	int *request = malloc(n*sizeof(int));
	while (1) {
		/* Generate request */
		generate_request(i, request);
		while (!resource_request(i, request)) {
			/* Wait */
			Sleep(100);
		}
		/* Generate release */
		generate_release(i, request);
		/* Release resources */
		resource_release(i, request);

		/* Wait */
		Sleep(1000);
	}
	printf("Process %i finished",i);
	free(request);
}

/* Allocate memory needed for runtime */
void alloc_memory(){
	s = (State *) malloc(2 * sizeof(int *) + 3 * sizeof(int **));
	s -> resource 	= 	malloc(n* sizeof(int));
	s -> available	=	malloc(n * sizeof(int));
	s -> max 	= 	malloc(m * sizeof(int *));
	s -> need 	=	malloc(m * sizeof(int *));
	s -> allocation = 	malloc(m * sizeof(int *));

	work 		=	malloc(n * sizeof(int));
	finish	 	=	malloc(m * sizeof(int)); 

	for(int i = 0; i < m; i++){
		s -> max[i] = malloc(n * sizeof(int));
		s -> need[i] = malloc(n * sizeof(int));
		s -> allocation[i] = malloc(n * sizeof(int));
	}
}


/* free memory allocation during runtime */
void free_memory(){
	for(int i = 0; i < m; i++){
		free(s -> max[i]);
		free(s -> need[i]);
		free(s -> allocation[i]);
	}
	free(s -> max);
	free(s -> need);
	free(s -> allocation);
	free(s -> available);
	free(s -> resource);
	free(finish);
	free(work);
	free(s);
}

int main(int argc, char* argv[])
{
	/* Get size of current state as input */
	int i, j;
	printf("Number of processes: ");
	scanf("%d", &m);
	printf("Number of resources: ");
	scanf("%d", &n);
	
	/* Allocate memory for state */
	alloc_memory();

	if (s == NULL) { printf("\nYou need to allocate memory for the state!\n"); exit(0); };

	/* Get current state as input */
	printf("Resource vector: ");
	for(i = 0; i < n; i++)
		scanf("%d", &s->resource[i]);
	printf("ok\n");
	printf("Enter max matrix: ");
	for(i = 0;i < m; i++)
		for(j = 0;j < n; j++)
			scanf("%d", &s->max[i][j]);
	printf("Enter allocation matrix: ");
	for(i = 0; i < m; i++)
		for(j = 0; j < n; j++) {
			scanf("%d", &s->allocation[i][j]);
		}
	printf("\n");

	/* Calcuate the need matrix */
	for(i = 0; i < m; i++)
		for(j = 0; j < n; j++)
			s->need[i][j] = s->max[i][j]-s->allocation[i][j];

	/* Calcuate the availability vector */
	for(j = 0; j < n; j++) {
		int sum = 0;
		for(i = 0; i < m; i++)
			sum += s->allocation[i][j];
			s->available[j] = s->resource[j] - sum;
	}

	/* Output need matrix and availability vector */
	printf("Need matrix:\n");
	for(i = 0; i < n; i++)
		printf("R%d ", i+1);
	printf("\n");
	for(i = 0; i < m; i++) {
		for(j = 0; j < n; j++)
			printf("%d  ",s->need[i][j]);
		printf("\n");
	}
	printf("Availability vector:\n");
	for(i = 0; i < n; i++)
		printf("R%d ", i+1);
	printf("\n");
	for(j = 0; j < n; j++)
		printf("%d  ",s->available[j]);
	printf("\n");

	/* If initial state is unsafe then terminate with error */
	
	if(safety_check()){
		/* Seed the random number generator */
		struct timeval tv;
		gettimeofday(&tv, NULL);
		srand(tv.tv_usec);
	 	
		/* init state mutex */
		pthread_mutex_init(&state_mutex, NULL);
 
		/* Create m threads */
		pthread_t *tid = malloc(m*sizeof(pthread_t));
		for (i = 0; i < m; i++)
			pthread_create(&tid[i], NULL, process_thread, (void *) (long) i);
	  
		/* Wait for threads to finish */
		pthread_exit(0);
		free(tid);
		free_memory();
	}
	else{
		printf("Unsafe state detected, now terminating\n");
		free_memory();
		return 1;
	}

}
