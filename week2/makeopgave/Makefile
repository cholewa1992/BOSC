all: main

main: main.o data.o io.o
	gcc main.o data.o io.o -o opg1.exe

main.o: main.c
	gcc -c main.c

data.o: data.c data.h
	gcc -c data.c

io.o: io.c io.h
	 gcc -c io.c

clean:
	rm -rf *o opg1.exe
