#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>



int p[2];

void
tareaA(void){
	
	// Variables
	
	int aleatorio, aleatorioAux;
	int i;
	
	// Fin Variables
	
	close(p[0]);
	
	srand(time(NULL));
	aleatorio = rand() % 10 + 1;
	for (i = 0; i < aleatorio; i++){
		
		aleatorioAux = rand() % 10 + i;
		write(p[1], &aleatorioAux, sizeof(int));
		
	}
	
	aleatorioAux = -1;
	write(p[1], &aleatorioAux, sizeof(int));
	close(p[1]);
	
}

void
tareaB(void){
	
	// Variables
	
	int num;
	int nr;
	
	// Fin Variables
	
	close(p[1]);
	nr = read(p[0], &num, sizeof(int));
	
	while(1){
		
		if (nr == 0){
			
			break;
		
		} else if (num != -1){
			
			fprintf(stdout, "El numero es %d \n", num);
		
		}		
	}
	
	close(p[0]);
}

void
tareaC(void){
	
	exit(0);
	
}

int
main (void){
	
	// Variables
	
	pid_t PID;
	
	// Fin Variables
	
	pipe(p);
	
	PID = fork();
	
	if (PID < 0) {
		
		fprintf(stderr, "Error al crear el proceso hijo");
		exit(1);
		
	} else if (PID == 0){ // Proceso Hijo
		
		tareaB();
		exit(0);
		
	} else { // Padre
		
		tareaA();
		wait(NULL);
		tareaC();
		
	}
	
	exit(0);
}
