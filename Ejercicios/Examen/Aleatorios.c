#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

int p[2];

void
aleatorios (int sig){
	
	int aleatorio;	
		
	aleatorio = rand() % 10;
	write(p[1], &aleatorio, sizeof(int));

}

int
main (void){

	// Variables
	
	pid_t PID;
	
	char buffer[1024];
	
	int N;
	
	// Fin Variables

	pipe(p);
	
	PID = fork();
	
	if (PID < 0){ //Error al crear proceso hijo
		
		fprintf(stderr, "Error al crear proceso hijo");
		exit(1);
		
	} else if (PID == 0){ // Proceso Hijo
		
		close(p[0]);
		
		signal(SIGUSR1, aleatorios);
		
		for(;;){
			
			pause();
			
		}
		
		close(p[1]);
		exit(0);
		
	} else { // Proceso Padre
		
		close(p[1]);
		
		while (fgets(buffer, 1024, stdin) != NULL){
			
			kill(PID, SIGUSR1);
			
			read(p[0], &N, sizeof(int));
			printf("El numero aleatorio es: %d \n", N);
			
		}
		
		close(p[0]);
		kill(PID, SIGINT);
		wait(NULL);
		
	}
}
