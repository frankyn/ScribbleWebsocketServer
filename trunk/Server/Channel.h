#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <map>
#include <sstream>
#include <sys/epoll.h>
#include "./common/ThreadClass.h"
#include "./common/SemaphoreClass.h"
#include "./common/SocketClass.h"
#include "./common/Logger.h"
#include "./protocols/WSProtocol.h"
#include "./protocols/rfc_6455/RFC_6455.h"
#include "./scriptloader/ScriptLoader.h"
#include "./sqlite/SQLite.h"
#include "./mongodb/DBMongo.h"
#include "Connection.h"

#ifndef CHANNEL_HEADER
#define CHANNEL_HEADER
/*

Channel:
	Holds many connections
	Loads in LUA Script which handles interaction between clients
	Loads in MongoDB connection to save channel only data
	
Todo:
	Add in MongoDB Wrapper ( MongoDB )
	Add in the ability to check to see if settings were updated for channel.
*/

class Channel : public ThreadClass { 
	public:
		Channel ( std::string, std::string, std::string, unsigned );
		~Channel ( );
		//User limit reached
		void limitReached ( Connection * );
		void setStatus ( int );

		//Adding clients
		void addConnection ( Connection * );
		void removeConnection ( std::string );

		//Writing methods
		int sendTo ( std::string, std::string ); 
		int broadcast ( std::string, std::string );
		/*DB Calls for lua API*/
		/*
		int storeData ( std::string, std::string );
		std::string getData ( std::string );
		*/
		DBMongo * getDB();

		//Channel Information
		int usersConnected ( );
		std::string getName ( );

		/*LUA Module Interaction*/
		std::string currentScript ( );
		int updateScript ( std::string );
		
		/*Channel LUA API Callback*/
		static int luaBroadcast ( lua_State * );
		static int luaDisconnectUser ( lua_State * );
		static int luaSendTo ( lua_State * );
		
		/*Logger LUA API Callback*/
		static int luaLog ( lua_State * );
		
		/*DB LUA API Callback*/
		/*
			static int luaStore ( lua_State * );
			static int luaGet ( lua_State * );
		*/
		static int luaInsert ( lua_State * );
		static int luaUpdate ( lua_State * );
		static int luaQuery ( lua_State * );
		static int luaRemove ( lua_State * );
		static int luaCountQuery ( lua_State * );

	private:
		int eventsOccuring, eventFD, incomingFD, status;
		unsigned maxConnections;
		epoll_event ev, *eventsList; 
		ScriptLoader logicModule;

		std::string name; //channel name
		std::string scriptFile, scriptUpdate;
		SemClass sc;
		SQLite sqldb;
		DBMongo appDatabase;
		
		std::map<std::string, Connection*> connections; 

		//Thread Polymorphism
		void Setup();
		void Execute(void*);

};

#endif
