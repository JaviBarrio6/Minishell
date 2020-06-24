#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <time.h>

int p[2];

void
aleatorios (int sig){
	
	int N;
	
	if (sig == SIGUSR1){
		
		N = rand();
		write(p[1], &N, sizeof(int));
		
	} else if (sig == SIGUSR2){
		
		close(p[1]);
		exit(0);
		
	}
	
	
}

int
main (void){
	
	//Variables 
	
	pid_t PID;
	char buffer[1024];
	int N;
	
	//Fin Variables
	
	srand(time(NULL));
	pipe(p);
	
	PID = fork();
	
	if (PID < 0){ // Error
		
		fprintf(stderr, "Error al crear el proceso hijo");
		exit(1);
		
	} else if (PID == 0){ // Hijo
		
		close(p[0]);
		signal(SIGUSR1, aleatorios);
		signal(SIGUSR2, aleatorios);
		
		for (;;){
			pause();
		}
		
		exit(0);
		
		
	} else { // Padre
		
		close(p[1]);
		while (fgets(buffer, 1024, stdin) != NULL){
			
			kill(PID, SIGUSR1);
			read(p[0], &N, sizeof(int));
			printf("N: %d", N);
			
		}
		close(p[0]);
		kill(PID, SIGUSR2);
		wait(NULL);
		exit(0);
		
	}
	
	
}

