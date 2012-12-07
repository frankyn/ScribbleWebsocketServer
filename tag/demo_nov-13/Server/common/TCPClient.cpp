#include "TCPClient.h"

TCPClient::TCPClient ( ) : isConnected ( false ) {

}

TCPClient::TCPClient ( int desc ) {
	socketDesc = desc;
	isConnected = true;
}

TCPClient::~TCPClient ( ) {

}

void * TCPClient::get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int TCPClient::connect ( const char * ip, const char * port ) {
    int rv;
    char s[INET6_ADDRSTRLEN];
    addrinfo hints, *servinfo, *p;
    try {

	    memset(&hints, 0, sizeof hints);
	    hints.ai_family = AF_UNSPEC;
	    hints.ai_socktype = SOCK_STREAM;

	    if ((rv = getaddrinfo( ip, port, &hints, &servinfo)) != 0) {
	        throw "getaddrinfo: " + std::string ( gai_strerror(rv) );
	    }

	    // loop through all the results and connect to the first we can
	    for(p = servinfo; p != NULL; p = p->ai_next) {
	        if (tcpSocket(p->ai_family, p->ai_protocol) == -1) {
	            continue;
	        }
	        if (::connect(socketDesc, p->ai_addr, p->ai_addrlen) == -1) {
	            continue;
	        }

	        break;
	    }

	    if (p == NULL) {
	        throw "failed to connect";
	    }

	    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
	    
	    freeaddrinfo(servinfo); // all done with this structure

    } catch ( LogString e ) {

    }
    return 0;
}

int TCPClient::recv ( void * buffer, size_t len ) {
	return ::recv ( socketDesc, buffer, len, 0 );
}

int TCPClient::send ( const void * buffer, size_t len ) {
	return ::send ( socketDesc, buffer, len, 0 );
}