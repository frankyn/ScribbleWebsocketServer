#include <iostream>
#include <string>
#include <cassert>
#include "../WSProtocol.h"
#include "RFC_6455.h"

using namespace std;

int main ( ) { 
	WSProtocol * protocolTest = new RFC_6455();
	WSAttributes attributes;
	string exampleInput = "GET /chat HTTP/1.1\r\nHost: server.example.com\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\nOrigin: http://example.com\r\nSec-WebSocket-Protocol: chat, superchat\r\nSec-WebSocket-Version: 13";
	string exampleResponse = "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=\r\n\r\n";

	int verCheck = protocolTest->handshake( exampleInput , &attributes );
	
	assert ( verCheck == 0 );
	cout<<"Version Check Passed..."<<endl;
	assert ( exampleResponse.compare(attributes.response) == 0 );
	cout<<"Handshake Passed..."<<endl;
	/*
	
   	A single-frame unmasked text message

         0x81 0x05 0x48 0x65 0x6c 0x6c 0x6f (contains "Hello")

        A single-frame masked text message

         0x81 0x85 0x37 0xfa 0x21 0x3d 0x7f 0x9f 0x4d 0x51 0x58
         (contains "Hello")

        A fragmented unmasked text message

         0x01 0x03 0x48 0x65 0x6c (contains "Hel")

         0x80 0x02 0x6c 0x6f (contains "lo")
	
	*/
	string exampleWSMessage = "\x81\x05\x48\x65\x6c\x6c\x6f";
	string examplePayload = "Hello";
	
	string decodedMessage = protocolTest->decode ( exampleWSMessage );
	
	assert ( examplePayload.compare ( decodedMessage ) == 0 );
	cout<<"Decode Passed...1"<<endl;
	
	exampleWSMessage = "\x81\x85\x37\xfa\x21\x3d\x7f\x9f\x4d\x51\x58";
	examplePayload = "Hello";
	
	decodedMessage = protocolTest->decode ( exampleWSMessage );
	
	assert ( examplePayload.compare ( decodedMessage ) == 0 );
	cout<<"Decode Passed...2"<<endl;
	
	exampleWSMessage = "\x81\x05\x48\x65\x6c\x6c\x6f";
	std::string encodedMessage = protocolTest->encode ( decodedMessage );
	
	assert ( encodedMessage.compare ( exampleWSMessage ) == 0 );
	cout<<"Encode Passed...1"<<endl;

	return 0;
}
