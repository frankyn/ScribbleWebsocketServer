#include "TCPListener.h"

TCPListener::TCPListener() : isBinded(false), isListening(false) {
    tcpSocket();
}

TCPListener::~TCPListener() {

}

void TCPListener::resetPort(int desc) {
    int yes = 1;
    try {
        if (setsockopt(desc, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
            throw LogString("Unable to reuse port");
    } catch (LogString e) {
        Log("Socket: " + e);
    }
}

bool TCPListener::bind(int port) {
    portNumber = port;
    if (!isBinded) {
        try {
            //setNonBlocking(socketDesc);
            resetPort(socketDesc);

            info.sin_family = AF_INET;
            info.sin_port = htons(portNumber);
            info.sin_addr.s_addr = htonl(INADDR_ANY);
            memset(info.sin_zero, '\0', sizeof(info.sin_zero));
            if (::bind(socketDesc, (sockaddr * ) & info, sizeof(sockaddr)) == -1) {
                throw LogString("Unable to bind port");
            }
            isBinded = true;
        } catch (LogString e) {
            Log("Socket: " + e);
            return false;
        }
    }
    return true;
}

bool TCPListener::listen(int queue) {
    queueList = queue;
    if (isBinded && !isListening) {
        try {
            if (::listen(socketDesc, queue) == -1) {
                throw LogString("Unable to listen on port");
            }
            isListening = true;
        } catch (LogString e) {
            Log("Socket: " + e);
            return false;
        }
    }
    return true;
}

int TCPListener::accept() {
    return ::accept(socketDesc, &tempInfo, &tempLen);
}
