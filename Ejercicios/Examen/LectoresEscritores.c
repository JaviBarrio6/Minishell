#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <wait.h>
#include <stdio.h>
#include <unistd.h>


pthread_mutex_t mutex;
pthread_cond_t aLeer, aEscribir;
int lectores, escritores;


void
Lector(void){
	
	pthread_mutex_lock(&mutex);
	while(escritores){
		
		pthread_cond_wait(&aLeer, &mutex);
		
	}
	
	lectores++;
	pthread_mutex_unlock(&mutex);
	
	pthread_mutex_lock(&mutex);
	lectores--;
	
	while(!lectores){
		
		pthread_cond_signal(&aEscribir);
		
	}
	
	pthread_mutex_unlock(&mutex);
	pthread_exit(0);	
	
}

void
Escritor(void){
	
	pthread_mutex_lock(&mutex);
	while(escritores || lectores){
		
		pthread_cond_wait(&aEscribir, &mutex);
		
	}
	
	escritores++;
	pthread_mutex_unlock(&mutex);
	
	pthread_mutex_lock(&mutex);
	escritores--;
	
	
		
	pthread_cond_signal(&aEscribir);
	pthread_cond_broadcast(&aLeer);
		
	
	
	pthread_mutex_unlock(&mutex);
	pthread_exit(0);	
	
}
	
int
main (void){
	
	pthread_t th1, th2, th3, th4;
	
	pthread_mutex_init(&mutex, NULL);
	
	pthread_cond_init(&aLeer, NULL);
	pthread_cond_init(&aEscribir, NULL);
	
	pthread_create(&th1, NULL, Lector, NULL);
	pthread_create(&th2, NULL, Escritor, NULL);
	pthread_create(&th3, NULL, Lector, NULL);
	pthread_create(&th4, NULL, Escritor, NULL);
	
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	pthread_join(th3, NULL);
	pthread_join(th4, NULL);
	
	pthread_mutex_destroy(&mutex);
	
	pthread_cond_destroy(&aLeer);
	pthread_cond_destroy(&aEscribir);
	exit(0);
	
}
