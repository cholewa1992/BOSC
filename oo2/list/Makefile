all: fifo

OBJS = list.o main.o
LIBS = -pthread -std=c99

fifo: ${OBJS}
	gcc -o $@ ${LIBS} ${OBJS}

main.o: main.c
	gcc -c ${LIBS} main.c

list.o: list.c
	gcc -c ${LIBS} list.c

clean:
	rm -rf *o fifo
