/******************************************************************************
   main.c

   Implementation of a simple FIFO buffer as a linked list defined in list.h.

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "list.h"

#define NUMBER_OF_THREADS 3

// FIFO list;
List *fifo;

void *uselist(void *args);

int main(int argc, char* argv[])
{
	printf("Now starting\n");
	fifo = list_new();

	pthread_t tid[NUMBER_OF_THREADS];
	for(int i = 0; i < NUMBER_OF_THREADS; i++){
		printf("Now starting new thread number %i\n",i);
		pthread_create(&tid[i],NULL,uselist,(void *)fifo); //Starting the proces
	}

	for(;;){
		if(fifo -> len != 300)
		printf("number of elements %i\n", fifo->len);	
	}

	for(int i = 0; i < NUMBER_OF_THREADS; i++){
		pthread_join(tid[i],NULL);
	}

	return 0;
}

void *uselist(void *args){
	List *fifo = args;
	while(1){
			for(int i = 0; i < 100; i++)
				list_add(fifo, node_new_str("s"));

			for(int i = 0; i < 100; i++){
				Node *n = list_remove(fifo);
				if(n == NULL){ printf("Error no elements in list\n"); exit(-1); }
			}
	}
	pthread_exit(0);	
}

