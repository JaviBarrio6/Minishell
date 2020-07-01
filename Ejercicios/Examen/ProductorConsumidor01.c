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
	
	pid_t PIDP;
	pid_t PIDC;
	
	char buffer[1024];
	
	int nr;
	int total;
	int p[2];
	int fd;
	
	// Fin Variables

	pipe(p);
	
	PIDP = fork();
	
	if (PIDP < 0){ // Error al crear el proceso hijo Productor
		
		fprintf(stderr, "Error al crear el proceso");
		exit(1);
		
	} else if (PIDP == 0){ // Proceso hijo Productor
		
		close(p[0]);
		
		fd = open("/tmp/datos.txt", O_RDONLY);
		
		if (fd < 0){
			
			fprintf(stderr, "Error al abrir fichero");
			exit(1);
			
		}
		
		while (1){
			
			nr = read(fd, buffer, 1024);
			
			if (nr < 0){
				
				break;
			
			}
			
			write(p[1], buffer, nr);
			
		} 
		close(p[1]);
		
	}
	
	PIDC = fork();
	
	if (PIDC < 0){ // Error al crear el proceso hijo Consumidor
		
		fprintf(stderr, "Error al crear el proceso");
		exit(1);
		
	} else if (PIDC == 0){ // Proceso hijo Consumidor
		
		close(p[1]);
		
		total = 0;
		
		while(1){
			
			nr = read(p[0], buffer, 1024);
			
			if (nr < 0){
				
				break;
				
			}
			
			total = total + nr;
			
		}
		
		close(p[0]);
		printf("Total: %d \n", total);
		exit(0);
		
	}
	
	sleep(60);
	kill(PIDP, SIGINT);
	kill(PIDC, SIGINT);
	exit(0);
	
}
