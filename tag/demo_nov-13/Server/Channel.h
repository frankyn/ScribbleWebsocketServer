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
#include "./scriptloader/ScriptLoader.h"
#include "./sqlite/SQLite.h"
#include "Connection.h"

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
		Channel ( std::string, std::string, unsigned );
		~Channel ( );
		//User limit reached
		void limitReached ( Connection * );
		void setStatus ( int );
		//Adding clients
		void addConnection ( Connection * );
		void removeConnection ( int );

		int broadcast ( std::string );
		int storeData ( std::string, std::string );
		
		std::string getData ( std::string );
		
		static int luaBroadcast ( lua_State* state ) {
			Channel * pthis = (Channel*)lua_touserdata( state, lua_upvalueindex(1));
			pthis->broadcast ( lua_tostring (state, -1 ) );
			std::cout<<"BROADCAST"<<std::endl;
			return 1;
		};

		static int luaStore ( lua_State* state ) {
			std::cout<<"STORE"<<std::endl;
			Channel * pthis = (Channel*)lua_touserdata ( state, lua_upvalueindex(1) );
			pthis->storeData ( lua_tostring ( state, 1 ), lua_tostring ( state, 0 ) );
			return 1;
		};

		static int luaGet ( lua_State * state ) {
			std::cout<<"GET"<<std::endl;
			Channel * pthis = (Channel*)lua_touserdata ( state, lua_upvalueindex(1) );
			std::string id = lua_tostring ( state, -1 );
			std::cout<<"id: "<<id<<std::endl;
			std::string data = pthis->getData ( id );
			std::cout<<"DATA:"<<data<<std::endl;
			lua_pushstring ( state, data.c_str() );
			return 1;
		};

		std::string getName ( );
	private:
		int eventsOccuring, eventFD, incomingFD, status;
		unsigned maxConnections;
		epoll_event ev, *eventsList; 
		ScriptLoader logicModule;

		std::string name; //channel name
		std::string scriptFile;
		SemClass sc;
		SQLite sqldb;
		
		std::list<Connection*> connections; 
		//Thread Polymorphism
		void Setup();
		void Execute(void*);

};

#endif
