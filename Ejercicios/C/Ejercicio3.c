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
	
	pid_t PID1;
	pid_t PID2;
	int fd;
	int p[2];
	char buffer[1024];
	int nr;
	int total;
	
	// Fin Variables
	
	pipe(p);
	
	PID1 = fork(); // Productor
	
	if (PID1 < 0){ // Error
		
		fprintf(stderr, "Error al crear el proceso");
		exit(1);
		
	} else if (PID1 == 0){ //Hijo Productor
		
		close(p[0]);
		
		fd = open("/tmp/datos", O_RDONLY);
		if (fd < 0){
			fprintf(stderr, "Error al abrir fichero");
			exit(2);
			
		} else {
			
			while(1){
				
				nr = read(fd, buffer, 1024);
				if (nr < 0){
					break;
				}
				
				write(p[1], buffer, nr);
				
			}
			
			close(p[1]);
			exit(0);
		}
	}
	
	PID2 = fork();
	
	if (PID2 < 0){ // Error
		
		fprintf(stderr, "Error");
		exit(1);
		
	} else if (PID2 == 0){ // Hijo Consumidor
		
		close(p[1]);
		total = 0;
		
		while (1){
			
			nr = read(p[0], buffer, 1024);
			if (nr < 0){
				break;
			}
			
			total = total + nr;
			
		}
		
		close(p[0]);
		printf("Total: %d", total);
		exit(0);
		
	}
	
	close(p[0]);
		close(p[1]);
		
		sleep(10);
		printf("Muerte");
		kill(PID1, SIGINT);
		kill(PID2, SIGINT);
		wait(NULL);
		wait(NULL); 
		
		exit(0);
	
}
