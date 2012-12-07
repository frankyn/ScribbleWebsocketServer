#include <iostream>
#include <string>
#include "./common/TCPClient.h"
#include "./protocols/WSProtocol.h"

#ifndef CONNECTION_HEADER
#define CONNECTION_HEADER

class Connection {
	public:
		Connection(int, WSProtocol*);
		~Connection();
		
		int getSocket();
		int recv ( void * buffer, size_t len );
		int send ( const void * buffer, size_t len );
		int handshake ( std::string, WSAttributes * );
		std::string decode ( const std::string );
		std::string encode ( const std::string );
		unsigned packetLength ( const std::string );
		int appendBuffer ( const std::string );
		int setBuffer ( const std::string );
		const std::string getBuffer ( );
	private:
		std::string buffer;
		TCPClient socket;
		WSProtocol * protocol;
};

#endif
