

pthread_mutex_t mutex;
pthread_cond_t noPaquetes, Paquetes;
int recogiendo, esperando;

void
Espera (void){
	
	pthread_mutex_lock(&mutex);
	
	while (recogiendo){
		
		pthread_cond_wait(&esperando, &mutex);
		
	}
	
	esperando++;
	pthread_mutex_unlock(&mutex);
	
	pthread_mutex_lock(&mutex);
	esperando--;
	
	if (!esperando){
		
		pthread_cond_signal(&recogiendo);
		
	}
	
	pthread_mutex_unlock(&mutex);
	pthread_exit(0);
	
}

void
Recoge (void){
	
	pthread_mutex_lock(&mutex);
	
	while (recogiendo || esperando){
		
		pthread_cond_wait(&recogiendo, &mutex);
		
	}
	
	recogiendo++;
	pthread_mutex_unlock(&mutex);
	
	pthread_mutex_lock(&mutex);
	recogiendo--;
	
	if (!recogiendo){
		
		pthread_cond_signal(&recogiendo);
		
	}
	
	pthread_mutex_unlock(&mutex);
	pthread_exit(0);
	
}

int
main (void){
	
	pthread_t th1, th2, th3, th4;
	
	pthread_mutex_init(&mutex, NULL);
	
	pthread_cond_init(&recogiendo, NULL);
	pthread_cond_init(&esperando, NULL);
	
	pthread_create(&th1, NULL, Recoge, NULL);
	pthread_create(&th2, NULL, Espera, NULL);
	pthread_create(&th3, NULL, Recoge, NULL);
	pthread_create(&th5, NULL, Espera, NULL);
	
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	pthread_join(th3, NULL);
	pthread_join(th4, NULL);
	
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&esperando);
	pthread_cond_destroy(&recogiendo);
	
	exit(0);
	
}
