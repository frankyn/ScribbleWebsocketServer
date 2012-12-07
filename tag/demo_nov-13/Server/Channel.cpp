#include <cerrno>
#include "Channel.h"

Channel::Channel ( std::string cname, std::string script, unsigned maxConn ) {
	this->name = cname;	
	scriptFile = script;
	maxConnections = maxConn;
	eventsList = new epoll_event[maxConnections];
	sqldb.connect ( "test.db" );
	logicModule.load ( this->scriptFile );
	logicModule.addProc ( &Channel::luaBroadcast, (void*)this,"broadcast" ); //Add broadcast
	logicModule.addProc ( &Channel::luaStore, (void*)this, "storeid" );
	logicModule.addProc ( &Channel::luaGet, (void*)this, "getid" );
	status = 1;
	ThreadClass::Start(this);
}

Channel::~Channel ( ) {
	connections.clear();
}

void Channel::setStatus ( int  s ) {
	status = s;
}

void Channel::Setup() {
	try{		
		Log ( "Channel " + this->name + " is being setup" );
		//logicModule.addFunction ( (lua_CFunction)&broadcast, "broadcast" );
		eventFD = epoll_create(10);//10 Doesn't mean anything after Linux Kern 2.6.8
		if ( eventFD<0 ) {
			  throw LogString("Unable to create an epoll Pipe");
		}
	}catch ( LogString e ) {
		Log ( "Listener: " + e );
		std::exit ( 0 );
	}
}

void Channel::Execute (void * arg) {
	try {
		int buffer_len = 0;
		char buffer[1024] = {0};
		Connection * conn;
		std::string msgTemp = "";
		unsigned msgLength = 0;
		Log ( "Channel " + this->name + " ready..." );
		
		while ( status ) {
			buffer_len = 0;
			conn = NULL;
			eventsOccuring = epoll_wait(eventFD, eventsList, 10, -1);
	
			if(eventsOccuring < 0){
				  throw "Unable to wait error occured";
			}
	
			for(int ce = 0; ce < eventsOccuring; ce++){
				conn = (Connection*) eventsList[ce].data.ptr;
				if((buffer_len=conn->recv(buffer,sizeof(buffer)-1))==0){
					Log ( "User disconnected" ); 
					removeConnection(ce);
					continue;
				}else{
					
					//Add incoming data for Connection to its own personal buffer.
					if ( conn->appendBuffer ( std::string ( buffer, buffer_len ) ) > 0 ) {
						//Call decode if return is empty string buffer isn't ready to be decoded.
						msgLength = conn->packetLength ( conn->getBuffer( ) );
						if ( msgLength <= conn->getBuffer().size() ) {
							//We have a complete packet waiting for us so we need clear it from the buffer.
							msgTemp = conn->getBuffer ( ).substr ( 0, msgLength );
							conn->setBuffer ( conn->getBuffer ( ).substr ( msgLength, conn->getBuffer().size() ) );	

						    //If incoming message is not empty then transmit to logicModule
						    //decode and check if not empty
						    if ( !( msgTemp = conn->decode( msgTemp ) ).empty() ) {
								//pass decoded message to logicModule script
								msgTemp = logicModule.call ( "onmessage", msgTemp );
							}
						}	
					}
				}
			}
		}
	}catch(LogString e) {
		Log ( "Listener: " + e );		
		std::exit(0);
	}
}

int Channel::broadcast ( std::string buffer ) {
	std::string msg;
	std::list<Connection*>::iterator it;
	for ( it = connections.begin(); it != connections.end(); it ++ ) {
		//encode message for every connection because the connection version could be different.
		msg = (*it)->encode ( buffer );
		(*it)->send ( msg.c_str(), msg.length() );
	}
	return 1;
}



void Channel::limitReached ( Connection * conn ) {
	//Disconnect user and send a User limit reached message
	std::string msg = "User limit reached!";
	msg = conn->encode ( msg );
	conn->send ( msg.c_str(), msg.length ( ) );
	delete conn;
}

void Channel::addConnection ( Connection * conn ) {
	try{
		if ( this->maxConnections <= connections.size ( ) ) {
			limitReached ( conn );
			return;
		} 
		ev.events = EPOLLIN | EPOLLET;
		ev.data.fd = conn->getSocket();
		ev.data.ptr = (void*)conn;
		if(epoll_ctl(eventFD, EPOLL_CTL_ADD, conn->getSocket(), &ev)!=0) {
			perror ( "epoll_ctl" );
			throw ( "Unable to add connection" );
		}
		connections.push_back ( conn );
		logicModule.call ( "onconnect", "" );
		std::cout<<"Connection has been added to "<<getName()<<std::endl;
	}catch(LogString e){
		Log("Listener: " + e);
	}

}

void Channel::removeConnection ( int ce ) {
	try {
		Connection * conn = (Connection*)eventsList[ce].data.ptr;
		if ( epoll_ctl ( eventFD, EPOLL_CTL_DEL, conn->getSocket(), NULL ) > 0 ){
			throw LogString ( "Unable to remove connection" );
		}
		logicModule.call ( "ondisconnect", "" );
		connections.remove ( conn ); //Connection is closed and free'd
	} catch ( LogString e ) {
		Log ( "Listener: " + e );
	}
}

int Channel::storeData ( std::string id, std::string value ) {
	std::string check = sqldb.query ( "select value from t1 where id='"+id+"'" );
	if ( check.empty() ) {
		sqldb.update ( "insert into t1 (id,value) values ('"+id+"', '"+value+"');" );
	}else {
		sqldb.update ( "update t1 SET value='"+value+"' WHERE id='"+id+"'" );
	}
	return 1;
}

std::string Channel::getData ( std::string id ) {
	std::string ret;
	ret = sqldb.query ( "select value from t1 where id ='"+id+"'" );
	return ret;
}

std::string Channel::getName ( ) {
	return this->name;
}
