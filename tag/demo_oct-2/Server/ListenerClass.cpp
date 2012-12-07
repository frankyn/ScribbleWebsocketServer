#include "ListenerClass.h"
#include "./common/Logger.h"
#include <cstdio>

ListenerClass::ListenerClass ( int port, int queue ) {
	channel = new Channel ( "example", 10 ); //Example channel
	listenerSocket = new SocketClass ( );
	listenerSocket->bindTo(port);
	listenerSocket->listenTo(queue);
	ThreadClass::Start(this);
	Log("Listener: Starting");
}

ListenerClass::~ListenerClass ( ) {
	delete channel;
	delete listenerSocket;
}

void ListenerClass::handleConnection(int desc){
	try{
		Log("Listener: User Connected");
		//Create special socket for protocol version
		//Send protocol response
		//Do a channel check		
		chselect = new ChannelSelector( desc , this->channel );

	}catch(LogString e){
		Log("Listener: "+ e);
	}
}

void ListenerClass::Setup(){
	try{
		
	}catch(LogString e){
		Log("Listener: " + e);
		std::exit(0);
	}
}

void ListenerClass::Execute(void * arg){
	try{
		Log("Listener: Started");
		for(;;){
			incomingFD = accept(listenerSocket->getSocket(), (sockaddr *)&listenerSocket->tempInfo, &listenerSocket->tempLen);
			handleConnection(incomingFD);
		}
	}catch(LogString e){
		Log("Listener: " + e);
		std::exit(0);
	}
}


