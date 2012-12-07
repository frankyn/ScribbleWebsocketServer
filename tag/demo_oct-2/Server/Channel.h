#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <list>
#include <sys/epoll.h>
#include "./common/ThreadClass.h"
#include "./common/SemaphoreClass.h"
#include "./common/SocketClass.h"
#include "./common/Logger.h"
#include "./protocols/WSProtocol.h"
#include "./protocols/rfc_6455/RFC_6455.h"
#ifndef CHANNEL_HEADER
#define CHANNEL_HEADER
/*

Channel:
	Holds many connections
	Loads in LUA Script which handles interaction between clients
	Loads in MongoDB connection to save channel only data

*/

class Channel : public ThreadClass { 
	public:
		Channel ( std::string, int );
		~Channel ( );
		//Adding clients
		void addConnection ( SocketClass * desc );
		void broadcast ( const void *, size_t, int );
	private:
		int eventsOccuring,eventFD,incomingFD, maxConnections;
		epoll_event ev, *eventsList; 
				
		std::string name; //channel name
		SemClass sc;
		
		std::list<SocketClass*> connections; 
		//Thread Polymorphism
		void Setup();
		void Execute(void*);

};

#endif
