#include "ListenerClass.h"
#include "./common/Logger.h"
#include <cstdio>


ListenerClass::ListenerClass ( int port, int queue, int selectors ) {
	try {
		int i;	
		maxSelectors = selectors;
		
		

		channels.insert ( channels.begin(), std::pair <std::string, Channel* >("example", new Channel ( "Example","logictests/example.lua", 10 ) ) ); //Example channel
		channels.insert ( channels.begin(), std::pair <std::string, Channel* >("default", new Channel ( "Default","logictests/default.lua", 10 ) ) ); //Default channel
	

		chselect = (ChannelSelector**) malloc ( sizeof(ChannelSelector*) * maxSelectors );
		for ( i = 0; i < maxSelectors; i ++ ) {
			chselect[i] = new ChannelSelector ( &connectionsWaiting, &channels );
		}

		if ( !listenerSocket.bind(port) ) {
			throw "Unable to bind port";
		}
		if ( !listenerSocket.listen(queue) ) {
			throw "Unable to listen on port";
		}
		
		status = 1;

		Start(this);
	} catch ( const char * e ) {
		std::cout<<"Listener: "<<e<<std::endl;
		std::exit ( 0 );
	}
}

ListenerClass::~ListenerClass ( ) {
	Wait();
	
	for ( int i = 0; i < maxSelectors; i ++ ) {
		delete chselect[i];
	}

	std::map<std::string, Channel* >::iterator it = channels.begin();
	for ( ; it != channels.end(); it ++ ) {
		channels.erase ( it );
	}
	channels.clear();

}

void ListenerClass::setStatus ( int s ) {
	
	for ( int i = 0; i < maxSelectors; i ++ ) {
		chselect[i]->setStatus ( s );
	}

	std::map<std::string, Channel* >::iterator it = channels.begin();
	
	for ( ; it != channels.end(); it ++ ) {
		it->second->setStatus ( s );
	}
	
	status = s;
}

void ListenerClass::handleConnection(int desc){
	try{
		//Create special socket for protocol version
		//Send it to the waiting line so it can be sent to the correct channel.
		connectionsWaiting.lock ();
		connectionsWaiting.insert ( desc );
		connectionsWaiting.unlock ();		
		connectionsWaiting.signal ();
	}catch(LogString e){
		Log("Listener: "+ e);
	}
}

void ListenerClass::Setup(){
	try{
		Log("Listener: Starting");

		
	}catch(LogString e){
		Log("Listener: " + e);
		std::exit(0);
	}
}

void ListenerClass::Execute(void * arg){
	try{
		Log("Listener: Started");
		while ( status ) {
			if ( ( incomingFD = listenerSocket.accept() ) > 0) {
				Log("Listener: New Connection");
				handleConnection(incomingFD);
			}
		}
	}catch(LogString e){
		Log("Listener: " + e);
		std::exit(0);
	}
}


