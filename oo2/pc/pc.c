#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include "list.h"

sem_t empty, full, mutex;
List *buffer;
int terminate = 0;
int c,p,buffer_size,np,products = 0;


void *producer(void *args);
void *consumer(void *args);


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
	sem_init(&mutex, 0, 1);

	buffer = list_new();	

	pthread_t tid[c+p];
	for(int i = 0; i < c; i++){
		pthread_create(&tid[i],NULL,consumer,(void *) i+1); //Starting the proces
	}
	for(int i = c; i < p+c; i++){
		pthread_create(&tid[i],NULL,producer,(void *) i-c+1); //Starting the proces
	}

	for(int i = c; i < c+p; i++){
		pthread_join(tid[i],NULL);
	}
	for(int i = 0; i < c; i++){
		pthread_detach(tid[i]);
	}
}


void *producer(void *args){
	int nr = args;
	while(1){
		sem_wait(&empty);
		sem_wait(&mutex);
		products++;
		if(products > np){
			sem_post(&mutex);
			return 0;
		}
		sem_post(&mutex);
		Node *n = node_new_str("string");
		list_add(buffer, n);
		printf("Producer %i produced %s. Items in buffer: %i (out of %i)\n",nr,n->elm,buffer->len,buffer_size);
		sem_post(&full);
		sleep(rand() % 1);
	}
	return 0;
}

void *consumer(void *args){
	int nr = args;
	while(!terminate){
		sem_wait(&full);
		Node *n = list_remove(buffer);
		printf("Consumer %i consumed %s. Items in buffer: %i (out of %i)\n",nr,n->elm,buffer->len,buffer_size);
		sem_post(&empty);
	}
	return 0;
}






