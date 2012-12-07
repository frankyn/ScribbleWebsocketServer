#ifndef SOCKET_HEADER
#define SOCKET_HEADER

#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

class SocketClass { 
	public:
		SocketClass ( );//New Socket
		SocketClass ( int );
		void setNonBlocking ( int );
		void resetPort ( int );
		bool bindTo ( int );
		bool listenTo ( int );
		int recv ( void * buffer, size_t len );
		int send ( const void * buffer, size_t len );
		int getSocket();
		~SocketClass ( );
		socklen_t tempLen;
		sockaddr tempInfo;
	protected:
		int socketDesc,portNumber,queueList;
		
	private:
		bool isBinded,isListening,isReady;
		sockaddr_in info;
};


#endif
