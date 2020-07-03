#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>


int
main (void){
	
	// Variables
	
	pid_t PIDP, PIDC;
	
	int p[2];
	int fd;
	int nr;
	int total;
	
	char buffer[1024];
	
	
	// Fin Variables
	
	pipe(p);
	
	PIDP = fork();
	
	if (PIDP < 0){ // Error
		
		fprintf(stderr, "Error al crear proceso hijo");
		exit(1);
		
	} else if (PIDP == 0){ // Proceso Productor
		
		close(p[0]);
		
		fd = open("/tmp/datos.txt", O_RDONLY);	
		
		if (fd < 0){
			
			fprintf(stderr, "Error al abrir el fichero");
			exit(1);
			
		}
		
		while(1){
			
			nr = read(fd, buffer, 1024);
			
			if (nr < 0){
				break;
			}
			
			write(p[1], buffer, nr);
			
		}
		
	}
	
	PIDC = fork();
	
	if (PIDC < 0){ // Error
		
		fprintf(stderr, "Error al crear proceso hijo");
		exit(1);
		
	} else if (PIDC == 0){ // Proceso Consumidor
		
		close(p[1]);
		
		total = 0;
		
		while(1){
			
			nr = read(p[0], buffer, 1024);
			
			if (nr < 0){
				break;
			}
			
			total = total + nr;
			
			fprintf(stdout, "El numero de caracteres es %d \n", total);
			
		}
		
	}
	
	close(p[0]);
	close(p[1]);
	
	sleep(60);
	kill(PIDP, SIGINT);
	kill(PIDC, SIGINT);
	
	exit(0);
}
