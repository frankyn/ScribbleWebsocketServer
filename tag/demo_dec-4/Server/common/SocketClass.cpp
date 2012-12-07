#include "SocketClass.h"

SocketClass::SocketClass ( ) {
	tempLen = sizeof(tempInfo);	
}

SocketClass::~SocketClass ( ) {
	close ( );
}

int SocketClass::close ( ) {
	if ( socketDesc ) {
		return ::close ( socketDesc );
	}
	return 0;
}

int SocketClass::tcpSocket ( ) {
	//Alias
	return newSocket ( AF_INET, SOCK_STREAM, 0 );
}

int SocketClass::tcpSocket ( int socket_family, int protocol ) {
	return newSocket ( socket_family, SOCK_STREAM, protocol );
}

int SocketClass::newSocket ( int socket_family, int socket_type, int protocol ) {
	try {
		socketDesc = ::socket ( socket_family, socket_type, protocol );
		if ( socketDesc < 0 ) {
			throw LogString ( "Unable to create a new socket" );
		}
		return socketDesc;
	} catch ( LogString e ) {
		Log ( "Socket: " + e );
		return -1;
	}

}

void SocketClass::setNonBlocking (int desc) {
	try{

		if(fcntl(desc,F_SETFL,O_NONBLOCK)<0) {
			throw LogString("Unable to set non-blocking");
		}
	}catch(LogString e){
		Log("Socket: " + e);
	}

}
int SocketClass::getSocket() { 
	return socketDesc;
}

std::string SocketClass::getSocket_str ( ) {
	std::stringstream stream;
	stream << socketDesc;
	return stream.str(); 
}

