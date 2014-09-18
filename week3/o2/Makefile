all: main

main: redirect.o
	gcc redirect.o -o main.exe

helloworld.o: helloworld.c 
	gcc -std=c99 -c helloworld.c

print.o: print.c
	gcc -std=c99 -c print.c

redirect.o: redirect.c redirect.h helloworld print
	gcc -c redirect.c redirect.h

clean:
	rm -rf *o *gch *log  main.exe forback helloworld print
