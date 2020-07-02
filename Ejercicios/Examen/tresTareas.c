#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

// Variables Globales

int p[2];

// Fin Variables Globales

void
tareaA (void){
	
	int i, rango, aleatorio;
	int fin = -1;
	
	close(p[0]);
	
	rango = rand();
		
	for (i = 0; i < rango; i++){
		
		aleatorio = rand();
		
		write(p[1], &aleatorio, sizeof(int));
		
	}
	
	write(p[1], &fin, sizeof(int));
	
	close(p[1]);
	
}

void
tareaB (void){
	
	int numero, nr;
	//int fin = -1;
	
	close(p[1]);
	
	while (1){
		
		nr = read(p[0], &numero, sizeof(int));
		
		if (nr < 0){
			
			break;
			
		}
		
		fprintf(stdout, "El numero aleatorio es %d \n", numero);
		
		if (numero == (-1)){
			
			break;
			
		}
		
	}
	
	close(p[0]);
	
}

void
tareaC (void){
	
	fprintf(stdout, "Fin de la Tarea C \n");
	
}


int
main (void){
	
	// Variables
	
	pid_t PID;
	
	// Fin Variables
	
	pipe(p);
	
	PID = fork();
	
	if (PID < 0){ // Error al crear proceso hijo
		
		fprintf(stderr, "Error al crear proceso hijo");
		exit(1);
		
	} else if (PID == 0){ // Proceso Hijo
		
		tareaB();
		
	} else { // Proceso Padre
		
		tareaA();
		wait(NULL);
		tareaC();
		exit(0);
		
	}
}
