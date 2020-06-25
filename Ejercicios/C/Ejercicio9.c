#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <wait.h>
#include <stdio.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define TOTAL_DATOS 100000

int nDatos;
int buffer[BUFFER_SIZE];

pthread_mutex_t mutex;
pthread_cond_t noLleno, noVacio;

void
productor(void){
	
	int i, dato;
	
	for (i = 0; i < TOTAL_DATOS; i++){
		
		pthread_mutex_lock(&mutex);
		
		while (nDatos == BUFFER_SIZE){
			
			pthread_cond_wait(&noLleno, &mutex);
			
		}
		
		buffer[i % BUFFER_SIZE] = dato;
		nDatos++;
		
		pthread_cond_signal(&noVacio);
		pthread_mutex_unlock(&mutex);
	}
	
	pthread_exit(NULL);
	
}

void
consumidor(void){
	
	int i, dato;
	
	for (i = 0; i < TOTAL_DATOS; i++){
		
		pthread_mutex_lock(&mutex);
		
		while (nDatos == 0){
			
			pthread_cond_wait(&noVacio, &mutex);
			
		}
		
		buffer[i % BUFFER_SIZE] = dato;
		nDatos--;
		
		pthread_cond_signal(&noLleno);
		pthread_mutex_unlock(&mutex);
	}
	
	pthread_exit(NULL);
	
}

int
main (void){
	
	pthread_t th1, th2;
	
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_cond_init(&noLleno, NULL);
	pthread_mutex_cond_init(&noVacio, NULL);
	
	pthread_create(&th1, NULL, productor, NULL);
	pthread_create(&th2, NULL, consumidor, NULL);
	
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&noVacio);
	pthread_cond_destroy(&noLleno);
	
	exit(0);	
}
