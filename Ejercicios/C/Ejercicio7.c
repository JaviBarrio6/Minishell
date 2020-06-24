#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <wait.h>
#include <stdio.h>
#include <unistd.h>

#define FILOSOFOS 5
pthread_mutex_t palillo [FILOSOFOS];

void
*th_filosofo(void *arg){
	
	int filosofoId = *(int*)arg;
	
	while(1){
		
		printf("Filosofo %d pensando \n", filosofoId);
		sleep((rand() % 10) + 1);
		
		printf("Filosofo %d quiere comer \n", filosofoId);
		pthread_mutex_lock(&palillo[filosofoId]);
		pthread_mutex_lock(&palillo[(filosofoId + 1) % 5]);
		
		printf("Filosofo %d comiendo \n", filosofoId);
		sleep((rand() % 10) + 1);
		
		pthread_mutex_unlock(&palillo[filosofoId]);
		pthread_mutex_unlock(&palillo[(filosofoId + 1) % 5]);
	}
	
}

void
*th_filosofoZurdo(void *arg){
	
	int filosofoId = *(int*)arg;
	
	while(1){
		
		printf("Filosofo %d pensando \n", filosofoId);
		sleep((rand() % 10) + 1);
		
		printf("Filosofo %d quiere comer \n", filosofoId);
		pthread_mutex_lock(&palillo[(filosofoId + 1) % 5]);
		pthread_mutex_lock(&palillo[filosofoId]);		
		
		printf("Filosofo %d comiendo \n", filosofoId);
		sleep((rand() % 10) + 1);
		
		pthread_mutex_unlock(&palillo[filosofoId]);
		pthread_mutex_unlock(&palillo[(filosofoId + 1) % 5]);
	}
	
}

int 
main (void){
	
	pthread_t filosofo[FILOSOFOS];
	srand(time(NULL));
	
	int uno = 1;
	int dos = 2;
	int tres = 3;
	int cuatro = 4;
	int cero = 0;
	int i;
	
	for (i = 0; i < FILOSOFOS; i++){
		
		pthread_mutex_init(&palillo[i], NULL);
		
	}
	
	pthread_create(&filosofo[0], NULL, th_filosofo, &cero);
	pthread_create(&filosofo[1], NULL, th_filosofo, &uno);
	pthread_create(&filosofo[2], NULL, th_filosofo, &dos);
	pthread_create(&filosofo[3], NULL, th_filosofo, &tres);
	pthread_create(&filosofo[4], NULL, th_filosofoZurdo, &cuatro);
	
	for (i = 0; i < FILOSOFOS; i++){
		
		pthread_join(filosofo[i], NULL);
		
	}
	
	exit(0);
}
