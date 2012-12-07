#include "SocketClass.h"

#ifndef TCP_LISTENER_HEADER
#define TCP_LISTENER_HEADER

class TCPListener : public SocketClass {
	public:
		TCPListener ( );
		~TCPListener ( );
		
		void resetPort ( int );
		bool bind ( int );
		bool listen ( int );
		int accept ( );
	private:
		int portNumber,queueList;
		bool isBinded, isListening;
};

#endif