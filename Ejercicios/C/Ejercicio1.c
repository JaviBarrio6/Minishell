#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>


void
mayusculas (char* buffer){
	
	int i;
	
	for (i = 0; i < strlen(buffer); i++){
		buffer[i] = toupper(buffer[i]);
	}
	
}

int
main (int argc, char **argv){
	
	//Variables 
	
	int PID;
	int p1[2];
	int p2[2];
	char buffer[1024];
	FILE* fr;
	FILE* fw;
	
	//Fin Variables
	
	pipe(p1);
	pipe(p2);
	
	PID = fork();
	
	if (PID < 0){ //Error
		
		fprintf(stderr, "Error al crear el proceso hijo \n");
		exit(1);
		
	} else if (PID == 0){ //Hijo
		
		close(p1[1]);
		close(p2[0]);
		
		fr = fdopen(p1[0], "r");
		fw = fdopen(p2[1], "w");
		
		while(fgets(buffer, 1024, fr) != NULL){
						
			mayusculas(buffer);
			fputs(buffer, fw);
			fflush(fw);
			
		}
		
		fclose(fr);
		fclose(fw);
		exit(0);
		
	} else { //Padre
		
		close(p1[0]);
		close(p2[1]);
		
		fw = fdopen(p1[1], "w");
		fr = fdopen(p2[0], "r");
		
		while(fgets(buffer, 1024, stdin) != NULL){
			
			fputs(buffer, fw);
			fflush(fw);
			fgets(buffer, 1024, fr);
			fputs(buffer, stdout);
			
		}
		
		fclose(fw);
		fclose(fr);
		wait(NULL);
		
		exit(0);
		
	}
	
	
}
