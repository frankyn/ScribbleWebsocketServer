#include <iostream>
#include <string>
#include <map>
#include "./common/SocketClass.h"
#include "./common/ThreadClass.h"
#include "./protocols/WSProtocol.h"
#include "./protocols/rfc_6455/RFC_6455.h"
#include "Connection.h"
#include "ConnectionsWaiting.h"
#include "Channel.h"

#ifndef CHANNEL_SELECTOR_HEADER
#define CHANNEL_SELECTOR_HEADER

/*
	Will read in protocol handshakes and then place client in the correct Channel Thread
*/

class ChannelSelector : public ThreadClass {
	public:
		ChannelSelector ( ConnectionsWaiting* , std::map<std::string, Channel*>);
		~ChannelSelector ( );
		void Setup ( );
		void Execute ( void * );
	private:
		std::map<std::string, Channel*> channels;
		ConnectionsWaiting * connectionsWaiting;
};

#endif
