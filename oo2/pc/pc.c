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

	sem_init(&empty, 0, buffer_size);
	sem_init(&full, 0, 0);
	sem_init(&psem, 0, 1);
	sem_init(&csem, 0, 1);

	buffer = list_new();	

	pthread_t tid[c+p];
	for(int i = 0; i < p; i++){
		pthread_create(&tid[i],NULL,producer,(void *) i+1);
	}
	for(int i = 0; i < c; i++){
		pthread_create(&tid[i+p],NULL,consumer,(void *) i+1);
	}

	for(int i = 0; i < c+p; i++){
		pthread_join(tid[i],NULL);
	}
}


void *producer(void *args){
	int nr = args;
	while(1){
		sem_wait(&psem);
		if(++produced > np){
			sem_post(&psem);
			return 0;
		}
		sem_wait(&empty);
		sem_post(&psem);
		Node *n = node_new_str("string");
		list_add(buffer, n);
		printf("Producer %i produced %s. Items in buffer: %i (out of %i)\n",nr,n->elm,buffer->len,buffer_size);
		sem_post(&full);
		rsleep(500);
	}
}

void *consumer(void *args){
	int nr = args;
	while(1){
		sem_wait(&csem);
		if(++consumed > np){
			sem_post(&csem);
			return 0;
		}
		sem_wait(&full);
		sem_post(&csem);
		Node *n = list_remove(buffer);
		printf("Consumer %i consumed %s. Items in buffer: %i (out of %i)\n",nr,n->elm,buffer->len,buffer_size);
		sem_post(&empty);
		rsleep(500);
	}
}
