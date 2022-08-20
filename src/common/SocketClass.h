#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include <sstream>
#include "Logger.h"

#ifndef SOCKET_CLASS_HEADER
#define SOCKET_CLASS_HEADER

class SocketClass {
public:
    SocketClass();

    ~SocketClass();

    int close();

    int getSocket();

    std::string getSocket_str();

    void setNonBlocking(int);

    void setTimeout(int, int, int);

protected:
    int tcpSocket();

    int tcpSocket(int, int);

    //void udpSocket ( );
    //void udpSocket ( int int );
    int socketDesc;
    socklen_t tempLen;
    sockaddr tempInfo;
    sockaddr_in info;
private:;

    int newSocket(int, int, int);
};

#endif
