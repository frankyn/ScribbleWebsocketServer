#ifndef THREAD_HEADER
#define THREAD_HEADER
#include <pthread.h>
#include <cerrno>
#include <cstdlib>

class ThreadClass{
	public:
		ThreadClass();
		~ThreadClass();
		int 		 Start(void * arg);
		void 		 Wait();
	protected:
		static void *EntryPoint(void*);
		void   		*getArg();
		void   		 setArg(void * arg);
		void   		 setStatus(bool);
		void		 wake();
		int    		 Run();
		bool   		 getStatus();
		  
		virtual void Setup()=0;
		virtual void Execute(void*)=0;
	private:
		pthread_t      threadID;
		pthread_attr_t threadAttributes;
		void 		 **threadRet;
		void          *argument;
		bool 		   running;
};
#endif