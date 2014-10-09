#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include "list.h"

sem_t empty, full, csem, psem;
List *buffer;
int c,p,buffer_size,np,consumed= 0, produced = 0;


void *producer(void *args);
void *consumer(void *args);

void rsleep(float wait_time_ms)
{
	wait_time_ms = ((float)rand())*wait_time_ms / (float)RAND_MAX;
	usleep((int) (wait_time_ms * 1e3f)); // convert from ms to us
}

int main(int argc, char* argv[]){
	if(argc != 5){
		printf("The systax is ./pc <consumers> <producers> <buffersize> <number of products>\n");
		exit(1);
	}
	if((c = atoi(argv[1])) <= 0){
		printf("The number of cosumers must be positive\n");
		exit(1);
	}
	if((p = atoi(argv[2])) <= 0){
		printf("The number of producers must be positive\n");
		exit(1);
	}
	if((buffer_size = atoi(argv[3])) <= 0){
		printf("The buffersize must be positive\n");
		exit(1);
	}
	if((np = atoi(argv[4])) <= 0){
		printf("The number of products must be positive\n");
		exit(1);
	}

	/* Seed the random number generator */
	struct timeval tv;
	gettimeofday(&tv, NULL);
	srand(tv.tv_usec);
	
	/* Initializing semaphores */
	sem_init(&empty, 0, buffer_size);
	sem_init(&full, 0, 0);
	sem_init(&psem, 0, 1);
	sem_init(&csem, 0, 1);

	/* Creating a new threadsafe list */
	buffer = list_new();	

	/* An array to contain thread ids */ 
	pthread_t tid[c+p];

	/* Starting producers */
	for(int i = 0; i < p; i++){
		pthread_create(&tid[i],NULL,producer,(void *) i+1);
	}

	/* Stating consumers */
	for(int i = 0; i < c; i++){
		pthread_create(&tid[i+p],NULL,consumer,(void *) i+1);
	}

	/* Waiting for all threads to be be done */
	for(int i = 0; i < c+p; i++){
		pthread_join(tid[i],NULL);
	}
}

void *producer(void *args){
	int nr = args;
	while(1){

		/* Incrementing produced with symaphore psem */
		sem_wait(&psem); //waiting for the given symaphore
		if(++produced > np){ //Incrementing *p and checking if the value is less than the maximum amount of products
			sem_post(&psem); //Releasing the lock before returning
			return 0; //Exiting the producer
		}
		sem_post(&psem); //Adding to the psem symaphore

		sem_wait(&empty); //Waiting if the list is full
		Node *n = node_new_str("string");
		list_add(buffer, n);
		printf("Producer %i produced %s. Items in buffer: %i (out of %i)\n",nr,n->elm,buffer->len,buffer_size);
		sem_post(&full); //Adding to the full symaphore
		rsleep(500); //Waiting a for a random range of 0 and 2*500ms
	}
}

void *consumer(void *args){
	int nr = args;
	while(1){

		/* Incrementing consumed with symaphore csem */
		sem_wait(&csem); //waiting for the given symaphore
		if(++consumed > np){ //Incrementing *p and checking if the value is less than the maximum amount of products
			sem_post(&csem); //Releasing the lock before returning
			return 0; //Exiting the consumer
		}
		sem_post(&csem); //Adding to the csem symaphore

		sem_wait(&full); //Waiting if list is empty
		Node *n = list_remove(buffer);
		printf("Consumer %i consumed %s. Items in buffer: %i (out of %i)\n",nr,n->elm,buffer->len,buffer_size);
		sem_post(&empty); //Adding to the empty symaphore
		rsleep(500); //Waiting a for a random range of 0 and 2*500ms
	}
}
