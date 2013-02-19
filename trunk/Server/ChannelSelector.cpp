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
		Log ( "Channel Selector: started" );
		while ( status ) { 
			newConnection = NULL;
			protocol = NULL;	
			if ( connectionsWaiting->wait ( ) ) {
				continue;
			}
			connectionsWaiting->lock ( );
			desc = connectionsWaiting->get ( );
			connectionsWaiting->unlock ( );	
			Log ( "Channel Selector: new connection" );	
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
					if ( channels->find( attributes.channel ) != channels->end() ) {
						//std::cout<< "Connection sent to: " << attributes.channel<<std::endl;
						Log ( "Channel Selector: Connection sent to: " + attributes.channel );
						(*channels) [ attributes.channel ]->addConnection ( newConnection );
						newConnection = NULL;
						protocol = NULL;
					} else {
						Log ( "Channel Selector: Channel Not found" );
						newConnection->close ( );
						//delete newConnection;//Protocol is freed within Connection()
						//newConnection = NULL;
						//protocol = NULL;
					}
					continue;
				}
			}
			//delete newConnection;//Protocol is freed within Connection()
			//newConnection = NULL;
			//protocol = NULL;
   		}
    } catch(std::exception& e){
		Log ( "Channel Selector: failed. --" + std::string(e.what ( )) );
    }
}
