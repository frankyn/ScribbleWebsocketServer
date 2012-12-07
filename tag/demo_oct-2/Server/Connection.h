#include <iostream>
#include "./common/SocketClass.h"
#include "./protocols/WSProtocol.h"

#ifndef CONNECTION_HEADER
#define CONNECTION_HEADER

class Connection {
	public:
		Connection();
		~Connection();
	private:
		SocketClass * socket;
		WSProtocol * protocol;
};

#endif
