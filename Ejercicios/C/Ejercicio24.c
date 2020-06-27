#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int p[2][2];

void
procesoPares(char* palabra){
	char fichero[1024];
	int fd;
	
	sprintf(fichero, "%s.txt", palabra);
	fd = creat(fichero, 0644);
	if (fd < 0){
		
		fprintf(stderr, "Error al crear el fichero");
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
procesoImpares(char* palabra){
	
	char fichero[1024];
	int fd;
	
	dup2(p[1][0], 0);
	close(p[1][0]);
	
	execlp("grep", "grep", palabra, NULL);
	exit(1);
}

int
main (int argc, char * argv[]){
	
	// Variables
	
	pid_t PIDP1, PIDP2;
	
	FILE * fr;
	FILE * fw[2];
	
	int i;
	
	char buffer[1024];
	
	// Fin Variables
	
	if (argc != 4) {
		
		fprintf(stderr, "Error en el numero de argumentos");
		exit(1);
		
	}
	
	for (i = 0; i < 2; i++){
		
		pipe(p[i]);
		
	}
	
	PIDP1 = fork();
	if (PIDP1 < 0){ // Error
		
		fprintf(stderr, "Error al crear el proceso hijo");
		exit(1);
		
	} else if (PIDP1 == 0){ // Proceso P1
		
		close(p[0][1]);
		close(p[1][0]);
		close(p[1][1]);
		
		procesoPares(argv[2]);
		exit(0);
		
	}
	
	PIDP2 = fork();
	if (PIDP2 < 0){ // Error
		
		fprintf(stderr, "Error al crear el proceso hijo");
		exit(1);
		
	} else if (PIDP2 == 0){ // Proceso P1
		
		close(p[0][0]);
		close(p[0][1]);
		close(p[1][1]);
		
		procesoImpares(argv[3]);
		exit(0);
		
	}
	
	close(p[0][0]);
	close(p[1][0]);
	
	fr = fopen(argv[1], "r");
	if (fr == NULL){
		
		fprintf(stderr, "Error \n");
		exit(1);
		
	} else {
		
		fw[0] = fdopen(p[0][1], "w");
		fw[1] = fdopen(p[1][1], "w");
		
		i = 0;
		while(fgets(buffer, 1024, fr) != NULL){
			
			fputs(buffer, fw[i]);
			fflush(fw[i]);
			i = i + 1 % 2;
		
		}		
		
		fclose(fw[0]);
		fclose(fw[1]);
		wait(NULL);
		wait(NULL);
	}
		
}
