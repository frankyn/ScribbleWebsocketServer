#include <cerrno>
#include "Channel.h"

Channel::Channel ( std::string cname, AppDB appDB, std::string script, unsigned maxConn ) {
	this->name = cname;	
	scriptFile = script;
	maxConnections = maxConn;
	eventsList = new epoll_event[maxConnections];
	
	/*Connect to database server*/
	/*Should probably make this configurable from outside.*/
	if ( !appDatabase.connect ( appDB.host ) ) {
		Log ( "Channel: Unable to connect to database" );
		throw "Unable to connect to database";
	}

	if ( appDB.auth ) {
		if ( !appDatabase.auth ( appDB.dbname, appDB.username, appDB.password ) ) {
			Log ( "Channel: Unable to authorize into to database" );
			throw "Unable to auth into database";
		}
	}

	appDatabase.useDB ( appDB.dbname );
	
	/*Loading in extra libraries*/
	logicModule.loadLib ( "lualibs/json.lua" );
	/*Logger LUA API*/
	logicModule.addProc ( &Channel::luaLog, (void*)this, "log" );
	/*Channel LUA API*/
	logicModule.addProc ( &Channel::luaBroadcast, (void*)this,"broadcast" ); //Add broadcast
	logicModule.addProc ( &Channel::luaDisconnectUser, (void*)this, "disconnectUser" );
	logicModule.addProc ( &Channel::luaSendTo, (void*)this, "sendTo" );
	/*MongoDB LUA API*/
	//logicModule.addProc ( &Channel::luaStore, (void*)this, "storeid" );
	//logicModule.addProc ( &Channel::luaGet, (void*)this, "getid" );
	logicModule.addProc ( &Channel::luaCountQuery, (void*)this, "countQuery" );
	logicModule.addProc ( &Channel::luaQuery, (void*)this, "query" );
	logicModule.addProc ( &Channel::luaInsert, (void*)this, "insert" );
	logicModule.addProc ( &Channel::luaRemove, (void*)this, "remove" );
	logicModule.addProc ( &Channel::luaUpdate, (void*)this, "update" );
	
	status = 1;
	ThreadClass::Start(this);
}

Channel::~Channel ( ) {
	connections.clear();
}

int Channel::usersConnected ( ) {
	return connections.size ( );
}

void Channel::setStatus ( int  s ) {
	status = s;
}

std::string Channel::currentScript ( ) {
	return scriptFile;
}

int Channel::updateScript ( std::string script ) {
	scriptUpdate = script;
	return 1;
}

void Channel::Setup() {
	try{		
		Log ( "Channel " + this->name + " is being setup" );

		eventFD = epoll_create ( 10 );//10 Doesn't mean anything after Linux Kern 2.6.8
		if ( eventFD<0 ) {
			  throw LogString("Unable to create an epoll Pipe");
		}
		logicModule.loadText ( this->scriptFile );
		logicModule.call ( "init" );
	}catch ( LogString e ) {
		Log ( "Listener: " + e );
		std::exit ( 0 );
	}
}

void Channel::Execute (void * arg) {
	while ( status ) {
	try {

		Connection * conn;
		std::string msgTemp = "";
		int buffer_len = 0;
		char buffer[1024] = {0};

		Log ( "Channel " + this->name + " ready..." );
		
		while ( status ) {
			buffer_len = 0;
			conn = NULL;
			eventsOccuring = epoll_wait(eventFD, eventsList, 10, -1);
			if(eventsOccuring < 0){
				  Log ( "Channel: Unable to wait 'epoll_wait' error" );
				  throw LogString ( "Unable to wait 'epoll_wait' error occured" );
			}

			for ( int ce = 0; ce < eventsOccuring; ce++ ) {
				conn = ( Connection* ) eventsList[ce].data.ptr;
				if ( ( buffer_len = conn->recv ( buffer, sizeof ( buffer ) - 1 ) ) == 0 ) {
					Log ( "User disconnected" ); 
					removeConnection ( conn->getID() );
					continue;
				}else{
					//Add incoming data for Connection to its own personal buffer.
					conn->appendBuffer ( std::string ( buffer, buffer_len ) );
				}
			}
		}

	}catch(LogString e) {
		Log ( "Listener: " + e );		
		//std::exit(0);
	}
	}
}

void Channel::doBeat ( ) {
	if ( !scriptUpdate.empty () ) {
		scriptFile = scriptUpdate;
		scriptUpdate = "";
		logicModule.loadText ( this->scriptFile );
		Log ( "Channel: Reloaded script" );
	}
			
	/* 
		Giving LUA Module onBeat method will give it life more 
		often without giving a user the commanding control.
	*/
	logicModule.call ("onBeat");
	//Handle any packets that are ready in the queue.
	handleConnectionBuffers ( );
}

void Channel::handleConnectionBuffers ( ) {
	std::map<std::string, Connection*>::iterator it;
	Connection * conn;
	std::string msgTemp;
	unsigned msgLength = 0;

	for ( it = connections.begin(); it != connections.end(); it ++ ) {
		conn = it->second;
		//Call decode if return is empty string buffer isn't ready to be decoded.
		msgLength = conn->packetLength ( conn->getBuffer ( ) );
		if ( msgLength != 0 && msgLength <= conn->getBuffer ( ).size ( ) ) {
			//We have a complete packet waiting for us so we need clear it from the buffer.
			msgTemp = conn->getBuffer ( ).substr ( 0, msgLength );
			conn->setBuffer ( conn->getBuffer ( ).substr ( msgLength, conn->getBuffer ( ).size ( ) ) );	

		    //If incoming message is not empty then transmit to logicModule
		    //decode and check if not empty
		    if ( !( msgTemp = conn->decode( msgTemp ) ).empty() ) {
				//pass decoded message to logicModule script
				SLArg args;
				args.push_back ( conn->getID () );
				args.push_back ( msgTemp );
				logicModule.call ( "onMessage", args );
			}
		}
	}
}

int Channel::broadcast ( std::string uid, std::string buffer ) {
	std::string msg;
	std::map<std::string, Connection*>::iterator it;
	for ( it = connections.begin(); it != connections.end(); it ++ ) {
		//encode message for every connection because the connection version could be different.
		if ( it->second->getID().compare ( uid ) != 0 ) {
			msg = it->second->encode ( buffer );
			it->second->send ( msg.c_str(), msg.length() );
		}
	}
	return 1;
}

int Channel::sendTo ( std::string uid, std::string buffer ) {
	std::map<std::string, Connection*>::iterator conn = connections.find ( uid );
	if ( conn!=connections.end() ) {
		std::string msg = conn->second->encode ( buffer );
		conn->second->send ( msg.c_str(), msg.length() );
	}

	return 1;
}

void Channel::limitReached ( Connection * conn ) {
	//Disconnect user and send a User limit reached message
	std::string msg = "User limit reached!";
	msg = conn->encode ( msg );
	conn->send ( msg.c_str(), msg.length ( ) );
	conn->close ( );
	delete conn;
}

void Channel::addConnection ( Connection * conn ) {
	try{
		//Check to see if max connections has been reached.
		if ( this->maxConnections <= connections.size ( ) ) {
			//If so then disconnect user.
			limitReached ( conn );
			return;
		} 
		int retEv = 0;
		ev.events = EPOLLIN | EPOLLET;
		ev.data.fd = conn->getSocket();
		ev.data.ptr = (void*)conn;
		if((retEv = epoll_ctl(eventFD, EPOLL_CTL_ADD, conn->getSocket(), &ev))!=0) {
			perror ( "epoll_ctl" );
			throw LogString ( "Unable to add connection" );
		}
		connections.insert ( std::pair<std::string, Connection*> (conn->getID(), conn) );
		SLArg args;
		args.push_back ( conn->getID() );
		logicModule.call ( "onConnect", args );
		Log ( getName() + ": Connection has been added" );
	}catch(LogString e){
		Log("Listener: " + e);
	}

}

void Channel::removeConnection ( std::string uid ) {
	try {
		if (  connections.find ( uid ) != connections.end() ) {
			Connection * conn = connections[uid];
			SLArg args;
			args.push_back ( uid );
			logicModule.call ( "onDisconnect", args );
			if ( epoll_ctl ( eventFD, EPOLL_CTL_DEL, conn->getSocket(), NULL ) > 0 ){
				throw LogString ( "Unable to remove connection" );
			}
			conn->close ( );
			connections.erase ( uid ); //Connection is closed and free'd
		}
	} catch ( LogString e ) {
		Log ( "Listener: " + e );
	}
}

DBMongo * Channel::getDB() {
	return &appDatabase;
}


std::string Channel::getName ( ) {
	return this->name;
}


int Channel::luaBroadcast ( lua_State* state ) {
	Channel * pthis = (Channel*)lua_touserdata( state, lua_upvalueindex(1));
	std::string connID = lua_tostring ( state, 1 );
	std::string msg = lua_tostring ( state, 2 );

	pthis->broadcast ( connID, msg );
	return 1;
};

int Channel::luaDisconnectUser ( lua_State * state ) {
	Channel * pthis = (Channel*)lua_touserdata ( state, lua_upvalueindex(1) );
	std::string id = lua_tostring ( state, -1 );
	pthis->removeConnection ( id );
	return 1;
};


int Channel::luaSendTo ( lua_State * state ) {
	Channel * pthis = (Channel*)lua_touserdata ( state, lua_upvalueindex(1) );
	std::string connID = lua_tostring ( state, 1 );
	std::string msg = lua_tostring ( state, 2 );
	pthis->sendTo ( connID, msg );
	return 1;
};

int Channel::luaLog ( lua_State * state ) {
	Log ( "Lua: " + std::string ( lua_tostring (state, -1 ) ) );
	return 1;
};

int Channel::luaInsert ( lua_State * state ) {
	Channel * pthis = (Channel*) lua_touserdata ( state, lua_upvalueindex (1) );
	DBMongo * db = pthis->getDB ( );
	std::string table = lua_tostring ( state, 1 );
	std::string newData = lua_tostring ( state, 2 );
	
	if ( !db->insert ( table, newData ) ) {
		//error
		lua_pushnumber ( state, 0 );
	}

	lua_pushnumber ( state, 1 );
	return 1;
}

int Channel::luaUpdate ( lua_State * state ) {
	Channel * pthis = (Channel*) lua_touserdata ( state, lua_upvalueindex (1) );
	DBMongo * db = pthis->getDB ( );
	std::string table = lua_tostring ( state, 1 );
	std::string where = lua_tostring ( state, 2 );
	std::string update = lua_tostring ( state, 3 );
	if ( !db->update ( table, where, update ) ) {
		//error
		lua_pushnumber ( state, 0 );
	}
	
	lua_pushnumber ( state, 1 );
	
	return 1;
}

int Channel::luaQuery ( lua_State * state ) {
	Channel * pthis = (Channel*) lua_touserdata ( state, lua_upvalueindex (1) );
	DBMongo * db = pthis->getDB ( );
	std::string table = lua_tostring ( state, 1 );
	std::string where = lua_tostring ( state, 2 );
	
	
	if ( !db->query ( table, where ) ) {
		lua_pushstring ( state, "Error occured" );
	} 
	
	lua_newtable ( state ); 

	for (int i = 0; db->hasNext ( ); i ++ ) {
		lua_pushnumber ( state, i );
		lua_pushstring ( state, db->next ().c_str() );
		lua_rawset ( state, -3 );
	}

	return 1;
}

int Channel::luaRemove ( lua_State * state ) {
	Channel * pthis = (Channel*) lua_touserdata ( state, lua_upvalueindex (1) );
	DBMongo * db = pthis->getDB ( );
	std::string table = lua_tostring ( state, 1 );
	std::string what = lua_tostring ( state, 2 );
	if ( !db->remove ( table, what ) ) {
		//error
		lua_pushnumber ( state, 0 );
	}

	lua_pushnumber ( state, 1 );
	return 1;
}

int Channel::luaCountQuery ( lua_State * state ) {
	Channel * pthis = (Channel*) lua_touserdata ( state, lua_upvalueindex (1) );
	DBMongo * db = pthis->getDB ( );
	std::string table = lua_tostring ( state, 1 );
	std::string query = lua_tostring ( state, 2 );
	int count = 0;
	if ( !(count=db->count ( table, query )) ) {
		//error
		lua_pushnumber ( state, 0 );
	}

	lua_pushnumber ( state, count );
	return 1;
}
