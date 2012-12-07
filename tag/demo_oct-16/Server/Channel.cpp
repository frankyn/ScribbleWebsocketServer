#include <cerrno>
#include "Channel.h"

Channel::Channel ( std::string name, std::string scriptFile, int maxConnections ) {
	this->name = name;	
	this->scriptFile = scriptFile;
	this->maxConnections = maxConnections;
	this->eventsList = new epoll_event[this->maxConnections];
	this->logicModule.load ( this->scriptFile );
	ThreadClass::Start(this);
}

Channel::~Channel ( ) {
	//ThreadClass::Wait();
}

void Channel::Setup() {
	try{		
		Log ( "Channel " + this->name + " is being setup" );
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
		Log ( "Channel " + this->name + " ready..." );
		
		for ( ;; ) {
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
					
					std::string decodedMsg = conn->decode(buffer);				
					std::string parsedMsg = logicModule.onmessage ( decodedMsg );
					std::string encodedMsg = conn->encode( parsedMsg );
					broadcast ( encodedMsg.c_str() , encodedMsg.length(), 0 );
				}
			}
		}
	}catch(LogString e) {
		Log ( "Listener: " + e );		
		std::exit(0);
	}
}

void Channel::broadcast ( const void * buffer, size_t len, int exception) {
	std::list<Connection*>::iterator it;
	for ( it = connections.begin(); it != connections.end(); it ++ ) {
		if ( (*it)->getSocket() != exception ) {
			(*it)->send ( buffer, len );
		}
	}
}

void Channel::addConnection ( Connection * conn ) {
	try{
		ev.events = EPOLLIN | EPOLLET;
		ev.data.fd = conn->getSocket();
		ev.data.ptr = (void*)conn;
		if(epoll_ctl(eventFD, EPOLL_CTL_ADD, conn->getSocket(), &ev)!=0) {
			perror ( "epoll_ctl" );
			throw ( "Unable to add connection" );
		}
		connections.push_back ( conn );
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
		
		close ( conn->getSocket() );
		connections.remove ( conn );
		delete conn;
	} catch ( LogString e ) {
		Log ( "Listener: " + e );
	}
}

std::string Channel::getName ( ) {
	return this->name;
}
