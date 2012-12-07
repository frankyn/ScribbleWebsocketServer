#include "RFC_6455.h"


RFC_6455::RFC_6455 ( ) {

}

RFC_6455::~RFC_6455 ( ) {

}
         
int RFC_6455::handshake (const std::string input, WSAttributes * response) {
	/*Grab Version*/
	std::string versionLookupStart ="Sec-WebSocket-Version: ";
	std::string versionLookupEnd = "\r\n";	
	int verStart = input.find( versionLookupStart );
	int verEnd = input.find ( versionLookupEnd, verStart );
	response->version = input.substr( verStart + versionLookupStart.length() , verEnd - (verStart+versionLookupStart.length()) );
	if ( response->version.compare("13") != 0 ) return -1;
	/*Grab Channel Name*/
	std::string channelLookupStart = "GET /";
	std::string channelLookupEnd = " HTTP/1.1";
	int channelStart = input.find( channelLookupStart ); 
	int channelEnd = input.find( channelLookupEnd, channelStart );
	response->channel = input.substr ( channelStart + channelLookupStart.length(), channelEnd - (channelStart + channelLookupStart.length()) );

	/*Create Server Response*/
	std::string GUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
	std::string handshake = "HTTP/1.1 101 Switching Protocols\r\n";
	handshake += "Upgrade: websocket\r\n";
	handshake += "Connection: Upgrade\r\n";
        handshake += "Sec-WebSocket-Accept: ";		
	std::string keyLookupStart = "Sec-WebSocket-Key: ";
	std::string keyLookupEnd = "\r\n";
	int keyStart = input.find( keyLookupStart );
	int keyEnd = input.find ( keyLookupEnd, keyStart );
	std::string inputKey = input.substr ( keyStart + keyLookupStart.length(), keyEnd - ( keyStart + keyLookupStart.length() ) ) + GUID;
	
	unsigned digest[5];
	unsigned char longDigest[20];
	SHA1 sha1;
	sha1.Reset();
	sha1.Input ( inputKey.c_str(), inputKey.length() );
	if ( !sha1.Result ( (unsigned * ) digest ) ) {
		std::cout<<"Sha1: Error ocurred creating the digest"<<std::endl;
		return -1;
	}

	for ( int i=0, j=0; i < 5; i++ ){
		longDigest[j++] = (digest[i] & 0xFF000000) >> 24;	
		longDigest[j++] = (digest[i] & 0x00FF0000) >> 16;
		longDigest[j++] = (digest[i] & 0x0000FF00) >> 8;
		longDigest[j++] = (digest[i] & 0xFF);
	}		

	response->response = handshake;
	response->response += base64_encode(longDigest, 20 );
	response->response += "\r\n\r\n";

	return 0;
}


std::string RFC_6455::decode ( const std::string input ) {
	std::string decodedInput;
	
	/*
	
   	A single-frame unmasked text message

         0x81 0x05 0x48 0x65 0x6c 0x6c 0x6f (contains "Hello")

        A single-frame masked text message

         0x81 0x85 0x37 0xfa 0x21 0x3d 0x7f 0x9f 0x4d 0x51 0x58
         (contains "Hello")
	
	*/
	const char * inputBytes = input.c_str(); 
	char maskedBit = inputBytes[1] & 0xF0;
	int payloadLength = inputBytes[1] & 0x0F;
	if ( maskedBit ) {
		//Masked Input
		int c = 0;
		int m = 2;		
		int offset = 6;
		while ( c < payloadLength ) {
			if ( m == 6 ) m = 2;
			decodedInput += inputBytes[c+offset] ^ inputBytes[m++];
			c++;
		}
	}else{
		//Not Masked
		int c = 0;
		int offset = 2;
		while ( c < payloadLength ) { 
			decodedInput += inputBytes[c+offset];	
			c++;
		}
	}	
	return decodedInput;
}

std::string RFC_6455::encode ( const std::string input ) {
	std::string encodedInput;

	int len = input.length();
	encodedInput += "\x81";
	encodedInput += (char)(len);
	encodedInput += input;

	return encodedInput;
}
