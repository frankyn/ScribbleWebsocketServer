#include <string>
#include <iostream>

#ifndef WSPROTOCOL_HEADER
#define WSPROTOCOL_HEADER


/*
    Interface Protocol Object to extend new protocols from.
    
    int handshake ( input, response ):
        Handshake will receive input from a new client that will check that the version of WebSocket being used and create a response based on the input. 
        The response is returned to response parameter.
        (int) return will give status
            : 0 - incorrect protocol input
            : 1 - protocol response was created
        
    string decode ( input ):
        Decode will receive input from a client and needs to be decoded based on the protocol encoding.
    
    string encode ( input ):
        Encode will receive input from the handler ( Server ) and needs to be encoded using the protocol encoding.
        
    Note*:
        If you feel that there should be changes please don't hesitate to make changes.

*/
struct WSAttributes { 
	std::string version;
	std::string channel;
	std::string response;

};
class WSProtocol{
	public:
		 virtual int handshake(const std::string input, WSAttributes * response) = 0;
		 virtual std::string decode(const std::string input) = 0;
        	 virtual std::string encode(const std::string input) = 0;
};

#endif
