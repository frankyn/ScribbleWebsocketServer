#include "./common/Logger.h"
#include "./common/SocketClass.h"
#include "./common/ThreadClass.h"
#include "./protocols/WSProtocol.h"
#include "Channel.h"
#include "Connection.h"
#include "ConnectionsWaiting.h"
#include "protocols/rfc_6455/WebSocket.h"
#include <iostream>
#include <map>
#include <string>

#ifndef CHANNEL_SELECTOR_HEADER
#define CHANNEL_SELECTOR_HEADER

/*
        Will read in protocol handshakes and then place client in the correct
   Channel Thread
*/

class ChannelSelector : public ThreadClass {
public:
  ChannelSelector(ConnectionsWaiting *, std::map<std::string, Channel *> *);

  ~ChannelSelector();

  void Setup();

  void Execute(void *);

  void setStatus(int);

private:
  int status;
  std::map<std::string, Channel *> *channels;
  ConnectionsWaiting *connectionsWaiting;
};

#endif
