#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

void
mayusculas (char* palabra){
	
	// Variables
	
	int i;
	
	// Fin Variables
	
	for (i = 0; i < strlen(palabra); i++){
		
		palabra[i] = toupper(palabra[i]);
		
	}
	
}

int
main (void){
	
	// Variables
	
	pid_t PID;
	
	int i;
	int p[2][2];
	
	char buffer[1024];
	
	FILE* fw;
	FILE* fr;
	
	// Fin Variables
	
	for (i = 0; i < 2; i++){
		
		pipe(p[i]);
		
	}
	
	PID = fork();
	
	if (PID < 0){ // Error
		
		fprintf(stderr, "Error al crear el proceso hijo");
		exit(1);
		
	} else if (PID == 0) { // Proceso Hijo
		
		close(p[0][1]);
		close(p[1][0]);
		
		fr = fdopen(p[0][0], "r");
		fw = fdopen(p[1][1], "w");
		
		while (fgets(buffer, 1024, fr) !=  NULL){
			
			mayusculas(buffer);
			fputs(buffer, fw);
			fflush(fw);
			
		}
		
		fclose(fr);
		fclose(fw);
		
	} else { // Proceso Padre
		
		close(p[0][0]);
		close(p[1][1]);
		
		fr = fdopen(p[1][0], "r");
		fw = fdopen(p[0][1], "w");
		
		while(fgets(buffer, 1024, stdin) != NULL){
			
			fputs(buffer, fw);
			fflush(fw);
			fgets(buffer, 1024, fr);
			
			fprintf(stdout, "En mayusculas: %s", buffer);
			
		}
		
		fclose(fr);
		fclose(fw);
		
	}
	
}
