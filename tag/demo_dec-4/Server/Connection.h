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
		std::string getSocket_str ( );
		std::string getID ( );

		int recv ( void * buffer, size_t len );
		int send ( const void * buffer, size_t len );
		int close ( );
		
		int handshake ( std::string, WSAttributes * );
		std::string decode ( const std::string );
		std::string encode ( const std::string );
		
		unsigned packetLength ( const std::string );
		int appendBuffer ( const std::string );
		int setBuffer ( const std::string );
		
		const std::string getBuffer ( );
		inline static void gen_random(char *s, const int len) {
		    static const char alphanum[] =
		        "0123456789"
		        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		        "abcdefghijklmnopqrstuvwxyz";

		    for (int i = 0; i < len; ++i) {
		        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
		    }

		    s[len] = 0;
		}
	private:
		std::string uniqueID;
		std::string buffer;
		
		TCPClient socket;
		WSProtocol * protocol;

};

#endif
