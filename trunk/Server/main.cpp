#include <iostream>
#include <string>
#include "./common/Logger.h"
#include "./ListenerClass.h"

int main(int argc, char ** args){
	try{
		
		ListenerClass LS;
		std::string h;

		while ( h.compare( "quit" ) != 0 ) {
			std::cout<<"Console: ";
			std::cin>>h;
			if ( h.compare ( "connected" ) == 0 ) {
				std::cout << "Connected: " << LS.usersConnected ( ) << std::endl;
			} else 
			if ( h.compare ( "channels" ) == 0 ) {
				std::cout<<"Available Channels:"<<std::endl
						 <<LS.availableChannels ( )<<std::endl;
			} else
			if ( h.compare ( "help" ) == 0 ) {
				std::cout<<"Commands:"<<std::endl
						 <<"help - list of commands"<<std::endl
						 <<"connected - connected user count"<<std::endl
						 <<"channels - list of channels"<<std::endl
						 <<"quit - exit program"<<std::endl;
			} else
			if ( h.compare ( "quit" ) != 0 ) {
				std::cout<<"'"<<h<<"' not a valid command. For list of commands type in 'help'"<<std::endl;
			} 
		}
		
		std::cout<<"Scribble is shutting down."<<std::endl;
		
		LS.setStatus ( 0 );
	} catch (const char * e) {
		std::cout<<"Error: "<<e<<std::endl;
	}

	return 0;
}
