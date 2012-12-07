#include "ChannelSelector.h"

ChannelSelector::ChannelSelector ( int desc, Channel * ch ) {
	this->connection = desc;
	this->ch = ch;
	this->protocol = new RFC_6455 ();
	ThreadClass::Start(this);
}

ChannelSelector::~ChannelSelector ( ) {
	

}

void ChannelSelector::Setup ( ) { 
	
}

void ChannelSelector::Execute ( void * arg ) {
	char buffer[1024];
	WSAttributes attributes;	
	SocketClass * newConnection = new SocketClass ( this->connection );
	newConnection->recv ( buffer, sizeof(buffer)/sizeof(char) );
	if ( this->protocol->handshake ( buffer, &attributes ) == 0 ){
		/*		
		std::cout<<"[Version] " << attributes.version << std::endl;
		std::cout<<"[Channel] " << attributes.channel << std::endl;
		std::cout<<"[Response] " << std::endl << attributes.response << std::endl;
		*/
		newConnection->send ( attributes.response.c_str(), attributes.response.length() );
	}
	this->ch->addConnection ( newConnection );
}
