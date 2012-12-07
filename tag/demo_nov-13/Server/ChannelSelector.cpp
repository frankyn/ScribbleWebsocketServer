#include "ChannelSelector.h"

ChannelSelector::ChannelSelector ( ConnectionsWaiting * waiting, std::map<std::string, Channel*> * masterChannelList ) {
	connectionsWaiting = waiting;
	channels = masterChannelList;
	status = 1;
	ThreadClass::Start(this);
}

ChannelSelector::~ChannelSelector ( ) {
	ThreadClass::Wait();
	channels = NULL;
}

void ChannelSelector::setStatus ( int s ) {
	status = s;
}

void ChannelSelector::Setup ( ) { 
	
}

void ChannelSelector::Execute ( void * arg ) {
    try{
		char buffer[1024] = {0};
		int bytes = 0, desc = 0;
		WSAttributes attributes;
		Connection * newConnection = NULL;
		WSProtocol * protocol = NULL;
		std::cout<<"Channel Selector: started"<<std::endl;	
		while ( status ) { 
			newConnection = NULL;
			protocol = NULL;	
			connectionsWaiting->wait ( );
			connectionsWaiting->lock ( );
			desc = connectionsWaiting->get ( );
			connectionsWaiting->unlock ( );	
			std::cout<<"Channel Selector: new connection"<<std::endl;	
			protocol = new RFC_6455();
			newConnection = new Connection ( desc, protocol );
			bytes = newConnection->recv ( buffer, sizeof(buffer)/sizeof(char) );
			if ( bytes ) {				
				if ( newConnection->handshake ( buffer, &attributes ) == 0 ){
					/*		
					std::cout<<"[Version] " << attributes.version << std::endl;
					std::cout<<"[Channel] " << attributes.channel << std::endl;
					std::cout<<"[Response] " << std::endl << attributes.response << std::endl;
					*/
					//std::cout<<"[Channel] " << attributes.channel << std::endl;
					newConnection->send ( attributes.response.c_str(), attributes.response.length() );
					std::cout<<attributes.channel<<std::endl;
					if ( channels->find( attributes.channel ) != channels->end() ) {
						//std::cout<< "Connection sent to: " << attributes.channel<<std::endl;
						(*channels) [ attributes.channel ]->addConnection ( newConnection );
					} else {
						std::cout<<"Channel Not found"<<std::endl;
						//std::cout<< "Channel not found" << channels.begin()->second->getName()<<std::endl;
					}
					continue;
				}
			}
			delete newConnection;//Protocol is freed within Connection()
			newConnection = NULL;
			protocol = NULL;
   		}
    } catch(...){
		Log("Socket:d ");
    }
}
