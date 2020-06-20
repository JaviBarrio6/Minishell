// Para compilar gcc  myShell.c libparser_64.a -static -o test

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#include "parser.h"

//Sección de Variables Globales
	
	tline * line; //Variable de tipo line
	const char changeDirectory[] = "cd"; //Variable mandato cd
	int *PIDS; //Variable para guardar los PIDs de los hijos
	
// Fin Sección de Variables Globales

void
executeCd (tline *line){
	//Sección de Variables Locales
	
	char *home;
	int error;
	
	//Fin de la Sección de Variables
	
	if ((line->commands[0].argc) == 1){ //Si solo han introducido cd, un argumento
		
		home = getenv("HOME"); //Capturamos la variable del entorno HOME
		
		error = chdir(home); //Cambiamos el directorio a home
		if (error < 0){ //Si ese directorio no existe
			fprintf(stderr, "%s No existe \n", home);
		}	
	} else { //Si nos dan el nombre del directorio al que queremos ir
		
		error = chdir(line->commands[0].argv[1]); //Cambiamos el directoria al que nos dan
		if (error < 0){ //Si ese directorio no existe
			fprintf(stderr, "%s No existe \n", line->commands[0].argv[1]);
		}		
	}
	
}

bool
isChangeDirectory(tline *line){
	
	if (strcmp(line->commands[0].argv[0], changeDirectory) == 0){ //Si introducimos cd como argumento
		
		return true; //Devolvemos TRUE
		
	} else {
		
		return false; //Si no, FALSE
		
	}
		
}

void
closePipes(int **p){
	
	for (int i = 0; i < (line->ncommands-1); i++){
		
		close(p[i][0]); //Cerramos el descriptor de lectura de fichero del pipe
		close(p[i][1]); //Cerramos el descriptor de escritura de fichero del pipe
	}	
}

void
executeLine(void){
	//Sección de Variables Locales
	
	int i; //Variable contadora
	int **p; //Variable para las pipes mediante memoria dinámica
	int fd;
	
	//Fin de la Sección de Variables
	
	p = malloc(sizeof(int*)*line->ncommands-1); 
	//Reservamos la memoria de un entero tantas veces como número de mandatos - 1
	for (i = 0; i < (line->ncommands-1); i++){ //Bucle de recorrido del número de mandatos
		p[i] = malloc(sizeof(int)*2); //Reservamos la memoria que ocupan dos enteros
	}
	
	for (i = 0; i < (line->ncommands-1); i++){ //Bucle de recorrido del número de mandatos
		pipe(p[i]); //Abrimos dos descriptores de fichero por cada comando introducido
	}
	
	for (i = 0; i < line->ncommands; i++){ //Bucle de recorrido del número de mandatos
		
		PIDS = malloc(sizeof(int)*line->ncommands);
		PIDS[i] = fork(); //Creamos un proceso hijo
		
		if (PIDS[i] < 0){ //Error al crear proceso hijo
			
			fprintf(stderr, "Error al crear proceso hijo"); //Notificación del error por la salida de error
			exit(1); //A partir de ahora el valor de error 1, se utilizará para el error al crear un proceso hijo
			
		} 
		
		if (PIDS[i] == 0){ //Proceso Hijo
			
			//Background
			
			if (!line->background){ //Si no tenemos background
				
				signal(SIGINT, SIG_DFL); //Habilitamos la señal SIGINT
				signal(SIGQUIT, SIG_DFL); //Habilitamos la señal SIGQUIT
				
			}
			
			//Fin Background
			
			//Redirecciones
			
			if ((i == 0) && (line->redirect_input)){ //Si es el primer mandato y tiene redirección de entrada
				
				fd = open(line->redirect_input, O_RDONLY); //Abrimos el fichero
				
				if (fd < 0){
					fprintf(stderr, "Error al abrir fichero"); //Notificación del error por la salida de error
					exit(3); //A partir de ahora el valor de error 3, se utilizará para el error al abrir un fichero
				} else {
					
					dup2(fd, 0); //Nuestra entrada estandar ahora es el fichero
					close(fd); //Cerramos el fichero
					
				}
				
			}
			
			if ((i == line->ncommands-1) && (line->redirect_output)){ //Si es el último y tiene redirección de entrada
				
				fd = creat(line->redirect_output, 0644); //Creamos el fichero
				
				if (fd < 0){
					fprintf(stderr, "Error al crear fichero"); //Notificación del error por la salida de error
					exit(4); //A partir de ahora el valor de error 4, se utilizará para el error al crear un fichero
				} else {
					
					dup2(fd, 1); //Nuestra salida estandar ahora es el fichero
					close(fd); //Cerramos el fichero
					
				}
				
			}
			
			if ((i == line->ncommands-1) && (line->redirect_error)){ //Si tiene redirección de Error
				
				fd = creat(line->redirect_error, 0644); //Creamos el fichero
				
				if (fd < 0){
					fprintf(stderr, "Error al crear fichero"); //Notificación del error por la salida de error
					exit(4); //A partir de ahora el valor de error 4, se utilizará para el error al crear un fichero
				} else {
					
					dup2(fd, 2); //Nuestra salida de error ahora es el fichero
					close(fd); //Cerramos el fichero
					
				}
				
			}
			
			
			//Fin de Redirecciones
			
			if (line->ncommands > 1){ //Si el numero de comandos es > 1
				
				if (i == 0){ //Si el mandato es el primero
					
					dup2(p[i][1], 1); //Nuestra salida estándar sera el pipe
					
					
				} else if (i == (line->ncommands-1)){ //Si el mandato es el último
					
					dup2(p[i-1][0], 0); //Nuestra entrada estándar sera el pipe donde ha escrito el anterior
					
					
				} else { //Si el mandato está entre el primero y el ultimo
					
					dup2(p[i-1][0], 0); //Nuestra entrada estándar sera el pipe donde ha escrito el anterior
					dup2(p[i][1], 1); //Nuestra salida estándar sera el pipe*/
					
				}
				
				closePipes(p);
			}
			
			execvp(line->commands[i].filename, line->commands[i].argv); //Ejecutamos el mandato i
			exit(2); //En caso de no ejecutar correctamente execvp abortamos ejecución
			
		}
	}
	
	closePipes(p); //Cerramos los pipes
	
	for (i = 0; i < (line->ncommands-1); i++){ //Bucle de recorrido del número de mandatos
		free(p[i]); //Liberamos la memoria
	}
	
	free(p); //Liberamos memoria
	
	for (i = 0; i < line->ncommands; i++){ //Esperamos a los hijos
		waitpid(PIDS[i], NULL, 0);
	}
	
}

int
main(void) {
	//Sección de Variables Locales
	
	char buffer[1024]; //Buffer
	char* path;
	
	//Fin de la Sección de Variables
	
	signal(SIGINT, SIG_IGN); //Deshabilitamos la señal SIGINT
	signal(SIGQUIT, SIG_IGN); //Deshabilitamos la señal SIGQUIT

	for (;;){ //Bucle Infinito
		if ((path = getcwd(NULL, 0)) == NULL){ //Obtenemos la ruta actual
			
			exit(5); //A partir de ahora el valor de error 5, se producira cuando no se pueda obtener la ruta actual
		
		} else {
			
			printf("\n~%s$ | Shell ==> ", path); //Mostramos el prompt con nuestra ruta actual
			
		}
		if (fgets(buffer, 1024, stdin) == NULL){ //Leo la linea y si es igual a Null salgo del bucle
			break;
		} else {
			line = tokenize(buffer); //Tokenizo, es decir, relleno la estructura de datos
			if ((line == NULL) || (strlen(buffer) == 1)){ //Si la entrada estandar está vacia o su longitud es 1
				continue; //Reinicia el bucle
			}
			printf("\n");
			if (isChangeDirectory(line)){ //Si introducen cd
				executeCd(line); //Ejecutar la función executeCd
			} else {
				
				executeLine(); //En caso contrario ejecutar la función executeLine
				
			}
		}
		printf("\n");
	}
	
	return 0;
}

