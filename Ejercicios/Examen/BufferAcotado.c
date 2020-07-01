#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <wait.h>
#include <stdio.h>
#include <unistd.h>

#define TAMANO_BUFFER 1024
#define TOTAL_DATOS 100000

pthread_mutex_t mutex;
pthread_cond_t noLleno, noVacio;

int buffer[TAMANO_BUFFER];
int numeroDatos;


void
Productor(void){
	
	int i, dato;
	
	dato = 0;
	
	for (i = 0; i < TOTAL_DATOS; i++){
		
		pthread_mutex_lock(&mutex);
		
		while(numeroDatos == TAMANO_BUFFER){
			
			pthread_cond_wait(&noLleno, &mutex);
			
		}
		
		buffer[i % TAMANO_BUFFER] = dato;
		
		numeroDatos++;
		
		pthread_cond_signal(&noVacio);
		pthread_mutex_unlock(&mutex);
		
	}
	
	pthread_exit(0);	
	
}

void
Consumidor(void){
	
	int i, dato;
	
	dato = 0;
	
	for (i = 0; i < TOTAL_DATOS; i++){
		
		pthread_mutex_lock(&mutex);
		
		while(numeroDatos == 0){
			
			pthread_cond_wait(&noVacio, &mutex);
			
		}
		
		buffer[i % TAMANO_BUFFER] = dato;
		
		numeroDatos--;
		
		pthread_cond_signal(&noLleno);
		pthread_mutex_unlock(&mutex);
		
	}
	
	pthread_exit(0);	
	
}
	
int
main (void){
	
	pthread_t th1, th2;
	
	pthread_mutex_init(&mutex, NULL);
	
	pthread_cond_init(&noVacio, NULL);
	pthread_cond_init(&noLleno, NULL);
	
	pthread_create(&th1, NULL, Productor, NULL);
	pthread_create(&th2, NULL, Consumidor, NULL);
	
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	
	pthread_mutex_destroy(&mutex);
	
	pthread_cond_destroy(&noVacio);
	pthread_cond_destroy(&noLleno);
	exit(0);
	
}
