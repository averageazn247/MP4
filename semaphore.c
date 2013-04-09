#include <pthread.h>
#include "semaphore.h"

/*--------------------------------------------------------------------------*/
/* Semaphore Class Functions  */
/*--------------------------------------------------------------------------*/
 

using namespace std;

Semaphore::Semaphore(int _val){
	value = _val;
	pthread_mutex_init(&m, NULL);
	pthread_cond_init(&c, NULL);
}

Semaphore::~Semaphore(){
	pthread_mutex_destroy(&m);
	pthread_cond_destroy(&c);
}

int Semaphore::p(){
	pthread_mutex_lock(&m);
	value--;
	if(value < 0){
		pthread_cond_wait(&c, &m);
	}
	pthread_mutex_unlock(&m);
}

int Semaphore::v(){
	pthread_mutex_lock(&m);
	value++;
	if(value <= 0){
		pthread_cond_signal(&c);
	}
	pthread_mutex_unlock(&m);
}
