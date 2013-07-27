#include "LockClass.h"

LockClass::LockClass(){	
	pthread_mutex_init(&plock, NULL);
}
LockClass::~LockClass(){
	pthread_mutex_destroy(&plock);
}
void LockClass::lock(){
	pthread_mutex_lock(&plock);	
}
void LockClass::unlock(){
	pthread_mutex_unlock(&plock);
	
}