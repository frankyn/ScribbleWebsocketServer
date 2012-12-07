#include "Connection.h"

Connection::Connection(int desc, WSProtocol * proto) : socket(SocketClass(desc)) {
	protocol = proto;
}

Connection::~Connection() {
	delete protocol;
}

int Connection::getSocket ( ) {
	return socket.getSocket ( );
}

int Connection::recv ( void * buffer, size_t len ) {
	return socket.recv ( buffer, len );
}

int Connection::send ( const void * buffer, size_t len ) {
	return socket.send ( buffer, len );
}

int Connection::handshake ( std::string input, WSAttributes * attributes ) {
	return protocol->handshake ( input , attributes);
}

std::string Connection::decode ( std::string input) {
	return protocol->decode ( input );
}

std::string Connection::encode ( std::string input) {
	return protocol->encode ( input );
}


