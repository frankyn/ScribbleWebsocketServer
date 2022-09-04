#if __APPLE__
#include <sys/event.h>
#include <sys/time.h>
#include <sys/types.h>
#elif __linux__
// linux
#include <sys/epoll.h>
#else
#error "Unknown compiler"
#endif

#include <ctime>
#include <iostream>
#include <list>
#include <map>

#include "./Channel.h"
#include "./ChannelSelector.h"
#include "./Connection.h"
#include "./ConnectionsWaiting.h"
#include "./common/TCPListener.h"
#include "./common/ThreadClass.h"
#include "./mysql/MySQL.h"
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

  std::map<std::string, Channel *> channels; // Only one channel for now
  ConnectionsWaiting connectionsWaiting;
  ChannelSelector **chselect;

  ScriptLoader config;

  MySQL appDB;
  TCPListener listenerSocket;
};

#endif
