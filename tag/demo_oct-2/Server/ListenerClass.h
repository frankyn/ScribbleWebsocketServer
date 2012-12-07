#ifndef LISTENER_HEADER
#define LISTERER_HEADER
#include <sys/epoll.h>
#include <iostream>
#include <list>
#include <map>
#include "./common/SocketClass.h"
#include "./common/ThreadClass.h"
#include "./ChannelSelector.h"
#include "./Channel.h"

class ListenerClass : public ThreadClass{
	public:
		ListenerClass(int,int);
		void handleConnection(int);
		~ListenerClass();
	private:
		void Setup();
		void Execute(void *);
		
		int incomingFD;
		Channel * channel; //Only one channel for now
		ChannelSelector * chselect;
		SocketClass * listenerSocket;
};

#endif
