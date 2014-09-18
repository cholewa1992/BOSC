all: main

main: pipe.o
	gcc pipe.o -o main.exe

helloworld.o: helloworld.c 
	gcc -std=c99 -c helloworld.c

print.o: print.c
	gcc -std=c99 -c print.c


pipe.o: pipe.c helloworld print
	gcc -c pipe.c pipe.h 

clean:
	rm -rf *o *gch *log  main.exe print helloworld printfile
