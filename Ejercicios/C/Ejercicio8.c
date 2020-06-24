#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <wait.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex;
pthread_cond_t condLeer, condEscribir;
int lectores, escritores;

void
lector(void){
	
	pthread_mutex_lock(&mutex);
	while (escritores){
		pthread_cond_wait(&condLeer, &mutex);
	}
	
	lectores++;
	pthread_mutex_unlock(&mutex);
	
	pthread_mutex_lock(&mutex);
	lectores--;
	if (!lectores){
		pthread_cond_signal(&condEscribir);
	}
	pthread_mutex_unlock(&mutex);
	pthread_exit(0);	
}

void
escritor(void){
	
	pthread_mutex_lock(&mutex);
	while ((escritores)||(lectores)){
		pthread_cond_wait(&condEscribir, &mutex);
	}
	
	escritores++;
	pthread_mutex_unlock(&mutex);
	
	pthread_mutex_lock(&mutex);
	escritores--;
	
	pthread_cond_signal(&condEscribir);
	pthread_cond_broadcast(&condLeer);
	
	pthread_mutex_unlock(&mutex);
	pthread_exit(0);	
}

int
main(void){
	
	pthread_t th1, th2, th3, th4;
	
	pthread_mutex_init(&mutex, NULL);
	
	pthread_cond_init(&condLeer, NULL);
	pthread_cond_init(&condEscribir, NULL);
	
	pthread_create(&th1, NULL, lector, NULL);
	pthread_create(&th2, NULL, escritor, NULL);
	pthread_create(&th3, NULL, lector, NULL);
	pthread_create(&th4, NULL, escritor, NULL);
	
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	pthread_join(th3, NULL);
	pthread_join(th4, NULL);
	
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&condLeer);
	pthread_cond_destroy(&condEscribir);
	
	exit(0);
}
