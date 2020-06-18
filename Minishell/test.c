// Para compilar gcc -ansi -g -Wall -Wextra -static test.c libparser_64.a -o test -std=c99
// o también gcc  test.c libparser_64.a -static -o test

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>

#include "parser.h"

//Sección de Variables Globales
	
	tline * line; //Variable de tipo line
	
// Fin Sección de Variables Globales

void
closePipes(int p[2]){
	
	close(p[0]); //Cerramos el descriptor de lectura de fichero del pipe
	close(p[1]); //Cerramos el descriptor de escritura de fichero del pipe
	
}

void
executeLine(void){
	//Sección de Variables Locales
	
	int i; //Variable contadora
	int p[2][2];
	pid_t PID; //Variable para guardar el PID del hijo
	
	
	//Fin de la Sección de Variables
	
	for (i = 0; i < line->ncommands; i++){ //Bucle de recorrido del número de comandos
		pipe(p[i]); //Abrimos dos descriptores de fichero por cada comando introducido 
	}
	
	for (i = 0; i < line->ncommands; i++){ //Bucle de recorrido del número de comandos
		
		PID = fork(); //Creamos un proceso hijo
		
		if (PID < 0){ //Error al crear proceso hijo
			
			fprintf(stderr, "Error al crear proceso hijo"); //Notificación del error por la salida de error
			exit(1); //A partir de ahora el valor de error 1, se utilizará para el error al crear un proceso hijo
			
		} else if (PID == 0){ //Proceso Hijo
			
			if (line->ncommands > 1){ //Si el numero de comandos es > 1
				
				if (i == 0){ //Si el mandato es el primero
					
					dup2(p[i][1], 1); //Nuestra salida estándar sera el pipe
					
				} else if (i == (line->ncommands -1)){ //Si el mandato es el último
					
					dup2(p[i-1][0], 0); //Nuestra entrada estándar sera el pipe donde ha escrito el anterior
					
				} else { //Si el mandato está entre el primero y el ultimo
					
					dup2(p[i-1][0], 0); //Nuestra entrada estándar sera el pipe donde ha escrito el anterior
					dup2(p[i][1], 1); //Nuestra salida estándar sera el pipe
					
				}
				
				closePipes(p[i]);
			}
			
			execvp(line->commands[i].filename, line->commands[i].argv); //Ejecutamos el mandato i
			exit(2); //En caso de no ejecutar correctamente execvp abortamos ejecución
			
		}
	}
	
	for (i = 0; i < line->ncommands; i++){
		wait(NULL);
	}
}

int
main(void) {
	//Sección de Variables Locales
	
	char buffer[1024]; //Buffer
	
	//Fin de la Sección de Variables

	for (;;){ //Bucle Infinito
		printf("Shell ==> ");
		if (fgets(buffer, 1024, stdin) == NULL){ //Leo la linea y si es igual a Null salgo del bucle
			break;
		} else {
			line = tokenize(buffer); //Tokenizo, es decir, relleno la estructura de datos
			printf("\n");
			executeLine();
		}
		printf("\n");
	}
	
	return 0;
}

