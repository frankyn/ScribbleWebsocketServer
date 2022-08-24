#if __APPLE__
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#elif __linux__
// linux
#include <sys/epoll.h>
#else
#   error "Unknown compiler"
#endif


#include <iostream>
#include <list>
#include <map>
#include <ctime>

#include "./common/TCPListener.h"
#include "./common/ThreadClass.h"
#include "./mysql/MySQL.h"
#include "./Connection.h"
#include "./ConnectionsWaiting.h"
#include "./ChannelSelector.h"
#include "./Channel.h"
#include "ScriptLoader.h"

#ifndef LISTENER_HEADER
#define LISTERER_HEADER

class ListenerClass : public ThreadClass {
public:
    ListenerClass();

    void handleConnection(int);

    ~ListenerClass();

    void setStatus(int);

    int usersConnected();

    std::string availableChannels();

    int checkForUpdates();

private:
    void Setup();

    void Execute(void *);

    void doBeat();

    int incomingFD, maxSelectors, status, queue, port;

    int elapseUpdateCheck;
    time_t lastUpdateCheck;

    std::map<std::string, Channel *> channels; //Only one channel for now
    ConnectionsWaiting connectionsWaiting;
    ChannelSelector **chselect;

    ScriptLoader config;

    MySQL appDB;
    TCPListener listenerSocket;
};

#endif
