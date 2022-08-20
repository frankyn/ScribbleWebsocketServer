#include <sys/epoll.h>
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
#include "./scriptloader/ScriptLoader.h"

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
