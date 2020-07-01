#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <wait.h>
#include <stdio.h>
#include <unistd.h>

#define FILOSOFOS 5
pthread_mutex_t palillos[FILOSOFOS];

void
*th_filosofo (void *arg){
	
	int fil_id = *(int *)arg;
	
	while (1){
		
		printf("Filosofo %d pensando \n", fil_id);
		sleep((rand() % 10) + 1);
		
		
		printf("Filosofo %d quiere comer \n", fil_id);
		pthread_mutex_lock(&palillos[fil_id]);
		pthread_mutex_lock(&palillos[(fil_id + 1) % 5]);
		
		printf("Filosofo %d comiendo \n", fil_id);
		sleep((rand() % 10) + 1);
		
		pthread_mutex_unlock(&palillos[fil_id]);
		pthread_mutex_unlock(&palillos[(fil_id + 1) % 5]);
	}
}


void
*th_filosofoZurdo (void *arg){
	
	int fil_id = *(int *)arg;
	
	while (1){
		
		printf("Filosofo %d pensando \n", fil_id);
		sleep((rand() % 10) + 1);
		
		
		printf("Filosofo %d quiere comer \n", fil_id);
		pthread_mutex_lock(&palillos[(fil_id + 1) % 5]);
		pthread_mutex_lock(&palillos[fil_id]);
		
		
		printf("Filosofo %d comiendo \n", fil_id);
		sleep((rand() % 10) + 1);
		
		pthread_mutex_unlock(&palillos[(fil_id + 1) % 5]);
		pthread_mutex_unlock(&palillos[fil_id]);
		
	}
}


int
main(void){
	
	pthread_t filosofo[FILOSOFOS];
	srand(time(NULL));
	
	int cero = 0, uno = 1, dos = 2, tres = 3, cuatro = 4;
	int i;
	
	for (i = 0; i < FILOSOFOS; i++){
		
		pthread_mutex_init(&palillos[i], NULL);
		
	}
	
	pthread_create(&filosofo[0], NULL, th_filosofo, &cero);
	pthread_create(&filosofo[1], NULL, th_filosofo, &uno);
	pthread_create(&filosofo[2], NULL, th_filosofo, &dos);
	pthread_create(&filosofo[3], NULL, th_filosofo, &tres);
	pthread_create(&filosofo[4], NULL, th_filosofoZurdo, &cuatro);
	
	for (i = 0; i < FILOSOFOS; i++){
		
		pthread_join(filosofo[i], NULL);
		
	}
	
	
	
}
