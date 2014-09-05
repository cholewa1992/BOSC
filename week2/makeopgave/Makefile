all: main

main: main.c data.o io.o
    gcc -c main.c

data: data.c data .h
    gcc -c data.c

io: io.c io.h
    gcc -c io.c

clean: rm -rf *o main