all: main

main: forback.o
	gcc forback.o -o main.exe

helloworld.o: helloworld.c 
	gcc -std=c99 -c helloworld.c

forback.o: forback.c forback.h helloworld
	gcc -c forback.c forback.h

clean:
	rm -rf *o *gch  main.exe forback helloworld
