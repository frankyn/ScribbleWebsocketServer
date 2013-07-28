#include "Connection.h"

Connection::Connection(int desc, WSProtocol * proto) : socket ( desc ) {
	protocol = proto;
	char randstr[11];
	gen_random ( randstr, 10 );
	uniqueID = randstr;
	buffer = "";

}

Connection::~Connection() {
	delete protocol;
}

int Connection::getSocket ( ) {
	return socket.getSocket ( );
}

std::string Connection::getID ( ) {
	return uniqueID;
}

std::string Connection::getSocket_str ( ) {
	return socket.getSocket_str ( );
}

int Connection::recv ( void * buffer, size_t len ) {
	return socket.recv ( buffer, len );
}

int Connection::send ( const void * buffer, size_t len ) {
	return socket.send ( buffer, len );
}

int Connection::close ( ) {
	return socket.close ();
}

int Connection::handshake ( std::string input, WSAttributes * attributes ) {
	return protocol->handshake ( input , attributes);
}

std::string Connection::decode ( const std::string input) {
	return protocol->decode ( input );
}

std::string Connection::encode ( const std::string input) {
	return protocol->encode ( input );
}

int Connection::appendBuffer ( const std::string input ) {
	buffer += input;
	return buffer.size();
}

int Connection::setBuffer ( const std::string input ) {
	buffer = input;
	return buffer.size();
}

unsigned long Connection::packetLength ( const std::string input ) {
	WSPacketLength pcktLength;
	protocol->packetLength ( input , &pcktLength );
	return pcktLength.length;
}

const std::string Connection::getBuffer ( ) {
	return buffer;
}
