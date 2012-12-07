#ifndef SEMAPHORE_CLASS_HEADER
#define SEMAPHORE_CLASS_HEADER
#include <cerrno>
#include <cstdlib>
#include <semaphore.h>

class SemClass{
	public: 
		  SemClass();
		  SemClass(unsigned int);
		  ~SemClass();
		  int value();
		  int wait();
		  int post();
	private:
		  sem_t s;
};

#endif