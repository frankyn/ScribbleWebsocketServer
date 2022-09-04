#include <iostream>
#include <list>
#include <string>

#include "./Connection.h"
#include "./common/LockClass.h"
#include "./common/SemaphoreClass.h"

#ifndef CONNECTIONS_WAITING_HEADER
#define CONNECTIONS_WAITING_HEADER

class ConnectionsWaiting {
public:
  ConnectionsWaiting();

  ~ConnectionsWaiting();

  void lock();

  void unlock();

  void insert(int);

  int get();

  void signal();

  void wait();

private:
  SemClass sem;
  LockClass pad;
  std::list<int> connectionsWaiting;
};

#endif
