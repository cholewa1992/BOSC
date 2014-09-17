#include <stdio.h>
#include <string.h>

#define MAXBUF 2048

void find(char *search, char *filename){
	FILE *file;
	file = fopen(filename,"r");
	char line[MAXBUF];
	int count = 0;	

	while(fgets(line,MAXBUF,file)){
		count++;	
		if(strstr(line,search))
			printf("line %d: %s",count, line);
	}
	fclose(file);
}


int main(int argc, char *argv[]){
	if(argc == 3){
		printf("Searching for '%s' in file: %s\n",argv[1],argv[2]);
	}else{
		printf("Too few arguments\n");
	}
	find(argv[1],argv[2]);
}
