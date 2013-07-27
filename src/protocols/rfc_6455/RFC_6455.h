#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include "../WSProtocol.h"
#include "./sha1/sha1.h"
#include "./base64/base64.h"

#ifndef RFC_6455_HEADER
#define RFC_6455_HEADER

/*
    RFC_6455 - based on http://tools.ietf.org/html/rfc6455

*/

class RFC_6455 : public WSProtocol{
    public:
         RFC_6455 ();
         ~RFC_6455 ();
         
         int handshake ( const std::string input, WSAttributes * response );
	 	 unsigned packetLength ( const std::string input );
	 	 std::string decode ( const std::string input );
         std::string encode ( const std::string input );
	
};

#endif
