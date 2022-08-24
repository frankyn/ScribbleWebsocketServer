#include <cassert>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include "Channel.h"


Channel::Channel(int channel_id,
                 std::string db_key,
                 std::string channel_name,
                 std::string script) {
    this->channelID = channel_id;
    this->channelDBKey = db_key;
    this->name = channel_name;
    scriptFile = script;
    maxConnections = 1000;
    eventsList = new epoll_event[maxConnections];


    /*Loading in extra libraries*/
    logicModule.loadLib("lualibs/json.lua");
    /*Logger LUA API*/
    logicModule.addProc(&Channel::luaLog, (void *) this, "log");
    /*Channel LUA API*/
    logicModule.addProc(&Channel::luaBroadcast,
                        (void *) this,
                        "broadcast");
    logicModule.addProc(&Channel::luaDisconnectUser,
                        (void *) this,
                        "disconnectUser");
    logicModule.addProc(&Channel::luaSendTo, (void *) this, "sendTo");

    this->status = 1;
    ThreadClass::Start(this);
}

Channel::~Channel() {
    connections.clear();
}

int Channel::usersConnected() {
    return connections.size();
}

void Channel::setStatus(int status) {
    this->status = status;
}

std::string Channel::currentScript() {
    return scriptFile;
}

int Channel::updateScript(std::string script) {
    scriptUpdate = script;
    return 1;
}

void Channel::Setup() {
    Log("Channel " + this->name + " is being setup");

    eventFD = epoll_create(10);
    assert(eventFD != -1);

    logicModule.loadText(this->scriptFile);
    logicModule.call("init");
}

void Channel::Execute(void *arg) {
    Connection *conn;
    std::string msgTemp = "";
    int buffer_len = 0;
    char buffer[1024] = {0};

    Log("Channel " + this->name + " ready...");

    while (status) {
        buffer_len = 0;
        conn = NULL;

        eventsOccuring = epoll_wait(eventFD, eventsList, 10, 30);
        // error occured...
        assert(eventsOccuring != -1);

        // No file descriptor became ready
        if (eventsOccuring == 0) {
            continue;
        }

        for (int ce = 0; ce < eventsOccuring; ++ce) {
            conn = (Connection *) eventsList[ce].data.ptr;
            if ((buffer_len = conn->recv(buffer, sizeof(buffer) - 1)) == 0) {
                Log("User disconnected");
                removeConnection(conn->getID());
                continue;
            } else {
                // Add incoming data for Connection to its own personal buffer.
                conn->appendBuffer(std::string(buffer, buffer_len));
                unsigned long msgLength = conn->packetLength(conn->getBuffer());

                if (msgLength != 0 && conn->packetComplete(conn->getBuffer())) {
                    // We have a complete packet waiting for us so we need clear it from the buffer.
                    msgTemp = conn->getBuffer().substr(0, msgLength);
                    conn->setBuffer(conn->getBuffer().substr(msgLength));
                    // If incoming message is not empty then transmit to logicModule
                    // decode and check if not empty
                    msgTemp = conn->decode(msgTemp);

                    if (!msgTemp.empty()) {
                        // pass decoded message to logicModule script
                        SLArg args;
                        args.push_back(conn->getID());
                        args.push_back(msgTemp);
                        logicModule.call("onMessage", args);
                    }
                }
            }
        }
        // Last thing to do before end of loop
        handleConnectionBuffers();
    }
}

void Channel::doBeat() {
    if (!scriptUpdate.empty()) {
        scriptFile = scriptUpdate;
        scriptUpdate = "";
        logicModule.loadText(this->scriptFile);
        Log("Channel: Reloaded script");
    }

    /*
      Giving LUA Module onBeat method will give it life more
      often without giving a user the commanding control.
    */
    logicModule.call("onBeat");

    // Handle any packets that are ready in the queue.
    handleConnectionBuffers();
}

void Channel::handleConnectionBuffers() {
    std::map<std::string, Connection *>::iterator it;
    Connection *conn;
    std::string msgTemp;

    for (it = connections.begin(); it != connections.end(); ++it) {
        conn = it->second;

        // Call decode if return is empty string buffer isn't ready to be decoded.
        unsigned long msgLength = conn->packetLength(conn->getBuffer());

        if (msgLength != 0 && conn->packetComplete(conn->getBuffer())) {
            msgTemp = conn->getBuffer().substr(0, msgLength);
            conn->setBuffer(conn->getBuffer().substr(msgLength));
            // If incoming message is not empty then transmit to logicModule
            //   decode and check if not empty
            msgTemp = conn->decode(msgTemp);

            if (!msgTemp.empty()) {
                // pass decoded message to logicModule script
                SLArg args;
                args.push_back(conn->getID());
                args.push_back(msgTemp);
                logicModule.call("onMessage", args);
            }
        }
    }
}

int Channel::broadcast(std::string uid, std::string buffer) {
    std::string msg;
    std::map<std::string, Connection *>::iterator it;

    for (it = connections.begin(); it != connections.end(); ++it) {
        // encode message for every connection because the
        //   connection version could be different.
        if (it->second->getID().compare(uid) != 0) {
            msg = it->second->encode(buffer);
            it->second->send(msg.c_str(), msg.length());
        }
    }
    return 1;
}

int Channel::sendTo(std::string uid, std::string buffer) {
    std::map<std::string, Connection *>::iterator conn = connections.find(uid);
    if (conn != connections.end()) {
        std::string msg = conn->second->encode(buffer);
        conn->second->send(msg.c_str(), msg.length());
    }

    return 1;
}

void Channel::limitReached(Connection *conn) {
    //Disconnect user and send a User limit reached message
    std::string msg = "User limit reached!";
    msg = conn->encode(msg);
    conn->send(msg.c_str(), msg.length());
    conn->close();
    delete conn;
}

void Channel::addConnection(Connection *conn) {
    //Check to see if max connections has been reached.
    if (this->maxConnections <= connections.size()) {
        //If so then disconnect user.
        limitReached(conn);
        return;
    }
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = conn->getSocket();
    ev.data.ptr = (void *) conn;

    int ret_val = epoll_ctl(eventFD, EPOLL_CTL_ADD, conn->getSocket(), &ev);
    assert(ret_val != -1);

    connections.insert(std::pair<std::string, Connection *>(conn->getID(), conn));
    SLArg args;
    args.push_back(conn->getID());
    logicModule.call("onConnect", args);
    Log(getName() + ": Connection has been added");
}

void Channel::removeConnection(std::string uid) {
    if (connections.find(uid) != connections.end()) {
        Connection *conn = connections[uid];
        SLArg args;
        args.push_back(uid);
        logicModule.call("onDisconnect", args);

        int ret_val = epoll_ctl(eventFD, EPOLL_CTL_DEL, conn->getSocket(), NULL);
        assert(ret_val != -1);

        conn->close();
        connections.erase(uid); //Connection is closed and free'd
    }
}

MySQL *Channel::getDB() {
    return &appDatabase;
}

std::string Channel::getName() {
    return this->name;
}

int Channel::getID() {
    return this->channelID;
}

std::string Channel::getDBKey() {
    return this->channelDBKey;
}

int Channel::luaBroadcast(lua_State *state) {
    Channel *pthis = (Channel *) lua_touserdata(state, lua_upvalueindex(1));
    std::string connID = lua_tostring(state, 1);
    std::string msg = lua_tostring(state, 2);

    pthis->broadcast(connID, msg);
    return 1;
};

int Channel::luaDisconnectUser(lua_State *state) {
    Channel *pthis = (Channel *) lua_touserdata(state, lua_upvalueindex(1));
    std::string id = lua_tostring(state, -1);
    pthis->removeConnection(id);
    return 1;
};


int Channel::luaSendTo(lua_State *state) {
    Channel *pthis = (Channel *) lua_touserdata(state, lua_upvalueindex(1));
    std::string connID = lua_tostring(state, 1);
    std::string msg = lua_tostring(state, 2);
    pthis->sendTo(connID, msg);
    return 1;
};

int Channel::luaLog(lua_State *state) {
    Log("Lua: " + std::string(lua_tostring(state, -1)));
    return 1;
};

// MySQL Extension for LUA

// Story a given value as a string with a key and Channel ID
int Channel::luaStore(lua_State *state) {
    Channel *pthis = (Channel *) lua_touserdata(state, lua_upvalueindex(1));
    MySQL *db = pthis->getDB();

    std::string key = lua_tostring(state, 1);
    std::string value = lua_tostring(state, 2);
    std::string dbKey = pthis->getDBKey();

    // Check to see if key is inside database
    db->query("SELECT * FROM appData WHERE key = '" + key + dbKey + "'");
    if (db->hasNext()) {
        // It has values
        // Update Value
        if (!db->exec("UPDATE appData SET value = '" +
                      value + "' WHERE key = '" +
                      key + dbKey + "'")) {
            // error
            lua_pushnumber(state, 0);
        }
    } else {
        // No values found
        // Insert a new Key with Value
        if (!db->exec("INSERT INTO appData  ( key, value ) VALUES ( '" +
                      key + dbKey + "','" + value + "' )")) {
            // error
            lua_pushnumber(state, 0);
        }
    }

    lua_pushnumber(state, 1);
    return 1;
}

// Story a given value as a string with a key and Channel ID
int Channel::luaGet(lua_State *state) {
    Channel *pthis = (Channel *) lua_touserdata(state, lua_upvalueindex(1));
    MySQL *db = pthis->getDB();

    std::string key = lua_tostring(state, 1);
    std::string value = lua_tostring(state, 2);
    std::string dbKey = pthis->getDBKey();

    // Check to see if key is inside database
    if (db->query("SELECT value FROM appData WHERE key = '" +
                  key + dbKey + "'")) {
        // error
        lua_pushnumber(state, 0);
    }
    if (db->hasNext()) {
        std::vector <std::string> data = db->next();
        lua_pushstring(state, data[0].c_str());
    } else {
        //Return successfully
        lua_pushnumber(state, 1);
    }

    return 1;
}

