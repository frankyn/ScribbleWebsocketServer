#ifndef LOCK_CLASS_HEADER
#define LOCK_CLASS_HEADER
#include <pthread.h>

class LockClass{
	public:
		LockClass();
	   ~LockClass();
		void lock();
		void unlock();
	private:
		pthread_mutex_t plock;
};

#endif