
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int p[2];

void
productor(char* name){
	
	// Variables
	
	FILE * fw;
	FILE * fr;
	char buffer [1024];

	
	// Fin Variables
	
	close(p[0]);
	
	fr = fopen(name, "r");
	if (fr < 0){
		
		fprintf(stderr, "Error al abrir el fichero");
		exit(1);
		
	}
	
	fw = fdopen(p[1], "w");
	
	while(fgets(buffer, 1024, fr) != NULL){
		
		fputs(buffer, fw);
		
	}
	
	fclose(fw);
	fclose(fr);
	exit(0);
		
	
}

void
consumidor (char* name){
	
	// Variables
	
	FILE * fr;
	FILE * fw;
	char buffer [1024];
	
	int numLinea, N;
	
	// Fin Variables
	
	numLinea = 0;
	
	close(p[1]);
	
	fw = fopen(name, "w");
	if (fw < 0){
		
		fprintf(stderr, "Error al abrir el fichero");
		exit (1);
		
	}
	
	fr = fdopen (p[0], "r");
	while (fgets(buffer, 1024, fr) != NULL){
		
		numLinea++;
		N = atoi(buffer);
		if ((N % 2) == 0){
			
			fprintf(stdout, "Numero: %d | Linea: %d", N, numLinea);
			
		}
	}
		fclose(fw);
		fclose(fr);
		exit(0);
	
}

int
main (int argc, char* argv[]){
	
	// Variables
	
	pid_t PIDP, PIDC;
	
	// Fin Variables
	
	if (argc != 3){
		
		fprintf(stderr, "Error en el numero de argumentos");
		exit(1);
		
	}
	
	pipe(p);
	
	PIDP = fork();
	if (PIDP < 0){
		
		fprintf(stderr, "Error al crear proceso hijo");
		exit(1);
		
	} else if (PIDP == 0){
		
		productor(argv[1]);
		
	}
	
	PIDC = fork();
	if (PIDC < 0){
		
		fprintf(stderr, "Error al crear proceso hijo");
		exit(1);
		
	} else if (PIDC == 0){
		
		consumidor(argv[2]);
		
	}
	
	sleep(100);
	kill(PIDP, SIGINT);
	kill(PIDC, SIGINT);
	exit(0);	
}
