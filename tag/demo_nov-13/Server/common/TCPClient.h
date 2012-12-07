#include "SocketClass.h"

#ifndef TCP_CLIENT_HEADER
#define TCP_CLIENT_HEADER

class TCPClient : public SocketClass {
	public:
		TCPClient ( );
		TCPClient ( int );
		~TCPClient ( );

		int connect ( const char *, const char * );
		int recv ( void *, size_t len );
		int send ( const void *, size_t len );

		void * get_in_addr ( struct sockaddr * );
	private:
		bool isConnected;
};

#endif