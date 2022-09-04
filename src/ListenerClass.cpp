#include "ListenerClass.h"
#include "./common/Logger.h"
#include <cassert>
#include <cstdio>

ListenerClass::ListenerClass() {
  config.load("config/config.lua");

  // Load in config file values
  config.focusVar("config");

  queue = config.getTableValue_int("connection_queue");
  maxSelectors = config.getTableValue_int("channel_selectors");
  elapseUpdateCheck = config.getTableValue_int("script_update_delay");
  port = config.getTableValue_int("port");

  appDB.connect(config.getTableValue_str("db_host"),
                config.getTableValue_str("db_username"),
                config.getTableValue_str("db_password"),
                config.getTableValue_str("db_database"));

  status = 1;
  Start(this);
}

ListenerClass::~ListenerClass() {
  Wait();

  for (int i = 0; i < maxSelectors; ++i) {
    chselect[i]->Wait();
    delete chselect[i];
  }

  std::map<std::string, Channel *>::iterator it = channels.begin();
  for (; it != channels.end(); ++it) {
    it->second->Wait();
  }
  channels.clear();
}

void ListenerClass::setStatus(int s) {
  for (int i = 0; i < maxSelectors; ++i) {
    chselect[i]->setStatus(s);
  }

  std::map<std::string, Channel *>::iterator it = channels.begin();

  for (; it != channels.end(); ++it) {
    it->second->setStatus(s);
  }

  status = s;
}

void ListenerClass::handleConnection(int desc) {
  // Create special socket for protocol version
  // Send it to the waiting line so it can be sent to the correct channel.
  connectionsWaiting.lock();
  connectionsWaiting.insert(desc);
  connectionsWaiting.unlock();
  connectionsWaiting.signal();
}

void ListenerClass::Setup() {
  // select apps.*, users.* from apps left join assoc
  //   on assoc.appid = apps.appid left join users
  //   on users.userid = assoc.userid
  int ret_val = appDB.query("select * from apps");
  assert(ret_val != 0);

  while (appDB.hasNext()) {
    std::vector<std::string> l = appDB.next();
    channels.insert(channels.begin(), std::pair<std::string, Channel *>(
                                          l[2], new Channel(atoi(l[0].c_str()),
                                                            l[2], l[1], l[3])));
  }

  chselect =
      (ChannelSelector **)malloc(sizeof(ChannelSelector *) * maxSelectors);
  for (int i = 0; i < maxSelectors; ++i) {
    chselect[i] = new ChannelSelector(&connectionsWaiting, &channels);
  }

  bool ret_bool = listenerSocket.bind(port);
  assert(ret_bool);

  ret_bool = listenerSocket.listen(queue);
  assert(ret_bool);

  listenerSocket.setTimeout(listenerSocket.getSocket(), 25, 0);
  Log("Listener: Starting");
}

void ListenerClass::Execute(void *arg) {
  Log("Listener: Started");
  while (status) {
    // Check for updated scripts and new channels.
    checkForUpdates();

    // Call doBeat to tell LUA Scripts to call onBeat
    doBeat();

    if ((incomingFD = listenerSocket.accept()) > 0) {
      Log("Listener: New Connection");
      handleConnection(incomingFD);
    }
  }
}

void ListenerClass::doBeat() {
  std::map<std::string, Channel *>::iterator it = channels.begin();
  for (; it != channels.end(); ++it) {
    it->second->doBeat();
  }
}

int ListenerClass::checkForUpdates() {
  time_t time_now = time(NULL);
  if (difftime(time_now, lastUpdateCheck) > elapseUpdateCheck) {
    lastUpdateCheck = time_now;

    int ret_val = appDB.query("select * from apps");
    assert(ret_val != 0);

    while (appDB.hasNext()) {
      std::vector<std::string> l = appDB.next();
      std::map<std::string, Channel *>::iterator it;
      if ((it = channels.find(l[2])) != channels.end()) {
        if (it->second->currentScript().compare(l[3]) != 0) {
          Log("Listener: Update was found for " + l[2]);
          it->second->updateScript(l[3]);
        }
      } else {
        channels.insert(
            channels.begin(),
            std::pair<std::string, Channel *>(
                l[2], new Channel(atoi(l[0].c_str()), l[2], l[1], l[3])));
      }
    }
  }
  return 1;
}

std::string ListenerClass::availableChannels() {
  std::string ret = "";
  std::map<std::string, Channel *>::iterator it = channels.begin();
  for (; it != channels.end(); ++it) {
    ret += it->second->getName() + "\n";
  }
  return ret;
}

int ListenerClass::usersConnected() {
  int users = 0;
  std::map<std::string, Channel *>::iterator it = channels.begin();
  for (; it != channels.end(); ++it) {
    users += it->second->usersConnected();
  }
  return users;
}
