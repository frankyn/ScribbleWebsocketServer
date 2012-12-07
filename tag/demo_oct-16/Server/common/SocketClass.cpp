#include "SocketClass.h"
#include "Logger.h"

SocketClass::SocketClass():isBinded(false),isListening(false){
	//Create a new socket to be used for something
	//Creating a TCP Socket*
	try{
		tempLen = sizeof(tempInfo);
		socketDesc = socket(AF_INET, SOCK_STREAM, 0);
		if(socketDesc<0)
			  throw LogString("Unable to create a new socket");
		isReady = true;
	}catch(LogString e){
			Log("Socket: " + e);
	}
}

SocketClass::SocketClass(int desc):isBinded(true),isListening(true){
	socketDesc = desc;
}

void SocketClass::setNonBlocking(int desc){
	try{

		if(fcntl(desc,F_SETFL,O_NONBLOCK)<0) {
			throw LogString("Unable to set non-blocking");
		}
	}catch(LogString e){
		Log("Socket: " + e);
	}

}
void SocketClass::resetPort(int desc){
	int yes = 1;
	try{
		if(setsockopt(desc,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))<0)
			throw LogString("Unable to reuse port");
	}catch(LogString e){
			Log("Socket: " + e);
	}
}
bool SocketClass::bindTo(int port){
	portNumber = port;
	if(isReady && !isBinded){
		try{
			//setNonBlocking(socketDesc);
			resetPort(socketDesc);
		 
			info.sin_family = AF_INET;
			info.sin_port = htons(portNumber);
			info.sin_addr.s_addr = htonl(INADDR_ANY);
			memset(info.sin_zero,'\0',sizeof(info.sin_zero));
			if(bind(socketDesc,(sockaddr *)&info, sizeof(sockaddr))<0)
				  throw LogString("Unable to bind port");
			isBinded = true;
		}catch(LogString e){
			Log("Socket: " + e);	
		}
	}
	return true;
}
bool SocketClass::listenTo(int queue){
	queueList = queue;
	if(isReady && isBinded && !isListening){
		try{
			if(listen(socketDesc,queue)<0)
				  throw LogString("Unable to listen on port");
			isListening = true;
		}catch(LogString e){
			Log("Socket: " + e);
		}
	}
	return true;
}
int SocketClass::getSocket() { 
	return socketDesc;
}

int SocketClass::recv ( void * buffer, size_t len ) {
	return ::recv ( socketDesc, buffer, len, 0 );
}

int SocketClass::send ( const void * buffer, size_t len ) {
	return ::send ( socketDesc, buffer, len, 0 );
}

SocketClass::~SocketClass(){
	if(socketDesc){
		close(socketDesc);
		//Log("Socket: cleaning up");
	}
}
