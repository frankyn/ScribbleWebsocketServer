#include <iostream>
#include <string>
#include <list>

#include "./common/SemaphoreClass.h"
#include "./common/LockClass.h"
#include "./Connection.h"

#ifndef CONNECTIONS_WAITING_HEADER
#define CONNECTIONS_WAITING_HEADER
class ConnectionsWaiting { 
	public:
		ConnectionsWaiting();
		~ConnectionsWaiting();

		void lock ( );
		void unlock ( );
		void insert ( int );
		int get ( );
		void signal ( );
		int wait ( );
	private:
		SemClass sem;
		LockClass pad;
		std::list<int> connectionsWaiting;

};

#endif

