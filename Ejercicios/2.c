


int
main (int argc, char* argv[]){
	
	// Region de Variables
	
	pid_t PID;
	
	int p[2][2];
	int fd[2];
	int contador;
	int fin;
	
	char buffer[1024];
	int existencias[200][2];
	int pedidos[200][2];
	char trozo[2];
	
	FILE * fr[2];
	FILE * fw[2];
	
	// Fin de Region de Variables

	if (argc != 2){
		
		fprintf(stderr, "Error en el numero de argumentos \n");
		exit(1);
		
	}
	
	pipe(p);
	
	PID = fork();
	
	if (PID < 0){ // Error
		
		fprintf(stderr, "Error al crear el proceso hijo");
		exit(1);
		
	} else if (PID == 0){ // Proceso Hijo
	
		close(p[0][1]);
		close(p[1][1]);
		
		contador = 0;
		
		fr[0] = fdopen(p[0][0], "r");
		fr[1] = fdopen(p[1][0], "r");
		
		while(fgets(buffer, 1024, fr[0]) != NULL){
			
			trozo = str_split(buffer, ' '); // Creo que en C split funcionaba así
			
			existencias[contador][0] = atoi(trozo[0]);
			existencias[contador][1] = atoi(trozo[1]);
			
		}
		
		contador = 0;
		
		while(fgets(buffer, 1024, fr[1]) != NULL){
			
			trozo = str_split(buffer, ' '); // Creo que en C split funcionaba así
			
			pedidos[contador][0] = atoi(trozo[0]);
			pedidos[contador][1] = atoi(trozo[1]);
			
		}
		
		for (i = 0; i < 200; i++){
			
			if ((existencias[i][1] - pedidos[i][1]) < 10){
				
				existencias[i][1] =  existencias[i][1] + 10;
 			
		}
		
		for (i = 0; i < 200; i++) {
			
			printf ("Codigo de Producto:  %d | Cantidad de Producto: %d \n", existencias[i][0], existencias[i][1]); 
			
		}
		
		close(fr[0]);
		close(fr[1]);
		exit(0);
		
	}	
	
	// Proceso Padre
	
	fin = -1;
	
	close(p[0][0]);
	close(p[1][0]);

	fd[0] = fopen (argv[1], "r");

	if (fd[0] == NULL){
	
		fprintf(stderr, "Error al abrir el fichero");
		exit (1);
	
	}

	fw[0] = fdopen(p[0][1], "w");
	fw[1] = fdopen(p[1][1], "w");

	while (fgets(buffer, 1024, fd[0]) != NULL){
	
		fputs(buffer, fw[0]);
		fflush(fw[0]);
			
	}

	
	fputs(fin, fw[0]);
	fflush(fw[0]);
	
	fd[1] = fopen (argv[2], "r");

	if (fd[1] == NULL){
	
		fprintf(stderr, "Error al abrir el fichero");
		exit (1);
	
	}
	
	while (fgets(buffer, 1024, fd[1]) != NULL){
	
		fputs(buffer, fw[1]);
		fflush(fw[1]);
			
	}

	fclose(fw[0]);
	fclose(fw[1]);
	
	wait(NULL);
	exit(0);
	
}

