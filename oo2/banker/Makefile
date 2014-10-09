all: banker

banker: banker.o
	gcc -o banker -pthread -std=c99 banker.o

banker.o: banker.c
	gcc -c -pthread -std=c99 banker.c

clean:
	rm -rf *o banker
