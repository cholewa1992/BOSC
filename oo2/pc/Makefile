all: pc

OBJS = list.o pc.o
LIBS = -pthread -std=c99

pc: ${OBJS}
	gcc -o $@ ${LIBS} ${OBJS}

pc.o: pc.c
	gcc -c ${LIBS} pc.c

list.o: list.c
	gcc -c ${LIBS} list.c

clean:
	rm -rf *o pc 
