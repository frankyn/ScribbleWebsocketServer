#include "ThreadClass.h"

ThreadClass::ThreadClass(){/*Constructor*/
	pthread_attr_init(&threadAttributes);
	pthread_attr_setdetachstate(&threadAttributes, PTHREAD_CREATE_JOINABLE);
	setStatus(false);
}
ThreadClass::~ThreadClass(){/*Destructor*/
	pthread_attr_destroy(&threadAttributes);
	pthread_exit(threadRet);
}
int ThreadClass::Start(void * arg){
	int threadRet;
	if(!getStatus()){
		setArg(arg);
		threadRet = pthread_create(&threadID, &threadAttributes,EntryPoint,(void*)this);
		 if(threadRet==-1){
			  switch(errno){
				case EAGAIN:
					//Insufficient memory to create another thread.
				break;
				case ENOMEM:
					//Insufficient memory for the thread's stack.
				break;
				case EINVAL:
					//Invalid attr
				break;
			  }
			  exit(0);	
		 }else{
			  setStatus(true);
			  return threadRet;
		 }
	}
	return 0;
}
void ThreadClass::Wait(){
	pthread_join(threadID,threadRet);
}
void ThreadClass::setStatus(bool status){
	running = status;
}
bool ThreadClass::getStatus(){
	return running;
}
void ThreadClass::setArg(void * arg){
	argument = arg;
}
void * ThreadClass::getArg(){
	return argument;
}
void * ThreadClass::EntryPoint(void * threadObject){
	ThreadClass * threadPointer = (ThreadClass*)threadObject;
	threadPointer->Run();
	return 0;
}
int ThreadClass::Run(){
	Setup();
	Execute(getArg());
	return 0;
}