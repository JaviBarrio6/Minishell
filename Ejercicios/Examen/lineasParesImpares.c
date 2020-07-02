#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>


// Variables Globales
	
	int p[2][2];
	
// Fin Variables Globales	

void
procesoPares (char* palabra){
	
	// Variables
	
	int fd;
	
	char buffer[1024];
	
	// Fin Variables
	
	close(p[0][1]);
	close(p[1][0]);
	close(p[1][1]);
	
	sprintf(buffer, "%s.txt", palabra);
	fd = creat(palabra, 0644);
	
	if (fd < 0){
		
		fprintf(stderr, "Error al crear fichero");
		exit(1);
		
	}
	
	dup2(fd, 1);
	close(fd);
	
	dup2(p[0][0], 0);
	close(p[0][0]);

	execlp("grep", "grep", palabra, NULL);
	exit(1);
		
}

void
procesoImpares (char* palabra){
	
	close(p[0][0]);
	close(p[0][1]);
	close(p[1][0]);
	
	dup2(p[1][0], 0);
	close(p[1][0]);

	execlp("grep", "grep", palabra, NULL);
	exit(1);
	
}


int
main (int argc, char* argv[]){

	// Variables
	
	int i;
	
	pid_t PIDP1, PIDP2, PIDP3;
	
	FILE * fw[2];
	FILE * fr;
	
	char buffer[1024];
	
	// Fin Variables 
	
	if (argc != 4){
		
		fprintf(stderr, "Error en el numero de argumentos");
		exit(1);
		
	}

	for (i = 0; i < 2; i++){
		
		pipe(p[i]);
		
	}
	
	PIDP1 = fork();
	
	if (PIDP1 < 0) { // Error al crear proceso 1
		
		fprintf(stderr, "Error al crear el Proceso 1");
		exit(1);
		
	}  else if (PIDP1 == 0){ // Proceso 1
		
		close(p[0][0]);
		close(p[1][0]);
		
		fr = fopen(argv[1], "r");
		
		if (fr == NULL){
			
			fprintf(stderr, "Error al abrir el fichero");
			exit(1);
			
		}
		
		fw[0] = fdopen(p[0][1], "w");
		fw[1] = fdopen(p[1][1], "w");
		
		i = 0;
		
		while(fgets(buffer, 1024, fr) != NULL){
			
			fputs(buffer, fw[i]);
			fflush(fw[i]),
			
			i = (i + 1) % 2;
			
		}
		
		fclose(fw[0]);
		fclose(fw[1]);
		exit(0);
		
	} 
	
	PIDP2 = fork();
	
	if (PIDP2 < 0){ // Error al crear Proceso 2
		
		fprintf(stderr, "Error al crear el Proceso 2");
		exit(1);
		
	} else if (PIDP2 == 0){ // Proceso 2
		
		procesoPares(argv[2]);
		exit(0);
		
	} 
	
	PIDP3 = fork();
	
	if (PIDP3 < 0){ // Error al crear Proceso 3
		
		fprintf(stderr, "Error al crear el Proceso 3");
		exit(1);
		
	} else if (PIDP3 == 0){ // Proceso 3
		
		procesoImpares(argv[3]);
		exit(0);
		
	} 
	
	wait(NULL);
	wait(NULL),
	wait(NULL);
}
