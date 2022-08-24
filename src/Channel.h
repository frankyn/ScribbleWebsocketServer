#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <map>
#include <sstream>

#if __APPLE__
#include <TargetConditionals.h>
#if TARGET_OS_MAC
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#else
#   error "Unknown Apple platform"
#endif
#elif __linux__
// linux
#include <sys/epoll.h>
#else
#   error "Unknown compiler"
#endif

#include "./common/ThreadClass.h"
#include "./common/SemaphoreClass.h"
#include "./common/SocketClass.h"
#include "./common/Logger.h"
#include "./protocols/WSProtocol.h"
#include "./protocols/rfc_6455/RFC_6455.h"
#include "ScriptLoader.h"
#include "./mysql/MySQL.h"
//#include "./mongodb/DBMongo.h"
#include "Connection.h"

#ifndef CHANNEL_HEADER
#define CHANNEL_HEADER
/*

Channel:
	Holds many connections
	Loads in LUA Script which handles interaction between clients
	Loads in MongoDB connection to save channel only data
	
Todo:
	Add in MongoDB Wrapper ( MongoDB )
	Add in the ability to check to see if settings were updated for channel.
*/
struct AppDB {
    int auth;
    std::string host;
    std::string dbname;
    std::string username;
    std::string password;
};
typedef struct AppDB AppDB;

class Channel : public ThreadClass {
public:
    Channel(int, std::string, std::string, std::string);

    ~Channel();


    //User limit reached
    void limitReached(Connection *);

    void setStatus(int);

    //Adding clients
    void addConnection(Connection *);

    void removeConnection(std::string);

    //Events
    void doBeat();

    //Handling Data
    void handleConnectionBuffers();

    //Writing methods
    int sendTo(std::string, std::string);

    int broadcast(std::string, std::string);

    //Get DB Handle
    MySQL *getDB();

    //Channel Information
    int usersConnected();

    std::string getName();

    std::string getDBKey();

    int getID();

    /*LUA Module Interaction*/
    std::string currentScript();

    int updateScript(std::string);

    /*Channel LUA API Callback*/
    static int luaBroadcast(lua_State *);

    static int luaDisconnectUser(lua_State *);

    static int luaSendTo(lua_State *);

    /*Logger LUA API Callback*/
    static int luaLog(lua_State *);

    /*DB LUA API Callback*/
    static int luaStore(lua_State *);

    static int luaGet(lua_State *);

private:
    int channelID;
    int eventsOccuring, eventFD, incomingFD, status;
    unsigned maxConnections;

#if __APPLE__
    struct kevent ev;
    int eventsList;
#elif __linux__
    // linux
    epoll_event ev, *eventsList;
#else
#   error "Unknown compiler"
#endif

    ScriptLoader logicModule;

    std::string name;
    std::string channelDBKey;
    std::string scriptFile, scriptUpdate;
    MySQL appDatabase;

    std::map<std::string, Connection *> connections;

    //Thread Polymorphism
    void Setup();

    void Execute(void *);
};

#endif
