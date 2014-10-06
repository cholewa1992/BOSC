all: banker

banker: banker.o
	gcc -o banker -pthread banker.o

banker.o: banker.c
	gcc -c -pthread banker.c

clean:
	rm -rf *o banker
