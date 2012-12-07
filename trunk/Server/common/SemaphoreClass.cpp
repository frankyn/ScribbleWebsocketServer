#include "SemaphoreClass.h"
SemClass::SemClass(){
	sem_init(&s,0,0);
}
SemClass::SemClass(unsigned int startCount){
	if(sem_init(&s,0,startCount)!=0){
		switch(errno){
			case EINVAL:
				//startCount too large and exceeds SEM_VALUE_MAX 
			break;
			case ENOSYS:
				//pshared is non-zero -- Argument 2 in sem_init()
			break;
		}
		exit(0);
	}
}
SemClass::~SemClass(){
	sem_destroy(&s);
}
int SemClass::value(){
	int val;
	sem_getvalue(&s,&val);
	return val;
}	
int SemClass::wait(){
	return sem_wait ( &s );
}

int SemClass::timedWait ( long nano_sec, time_t sec ) {
	timespec t = { nano_sec, sec };
	return sem_timedwait ( &s,  &t );
}

int SemClass::post(){
	return sem_post(&s);
}
