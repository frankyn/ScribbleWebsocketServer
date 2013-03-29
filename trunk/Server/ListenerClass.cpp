#include "ListenerClass.h"
#include "./common/Logger.h"
#include <cstdio>

/*

GO THROUGH EVERY POINTER DECLARATION AND SET TO NULL

*/
ListenerClass::ListenerClass ( int port, int queue, int selectors, int elapsedUpdate ) {
	try {
		maxSelectors = selectors;
		elapseUpdateCheck = elapsedUpdate;
		status = 1;
		this->port = port;
		this->queue = queue;
		appDB.connect ( "localhost" , "root", "blue23", "scribble" );
		Start(this);
	} catch ( const char * e ) {
		std::cout<<"Listener: "<<e<<std::endl;
		std::exit ( 0 );
	}
}

ListenerClass::~ListenerClass ( ) {
	Wait();
	
	for ( int i = 0; i < maxSelectors; i ++ ) {
		chselect[i]->Wait();
		delete chselect[i];
	}


	std::map<std::string, Channel* >::iterator it = channels.begin();
	for ( ; it != channels.end(); it ++ ) {
		it->second->Wait();
	}
	channels.clear();

}

void ListenerClass::setStatus ( int s ) {	
	for ( int i = 0; i < maxSelectors; i ++ ) {
		chselect[i]->setStatus ( s );
	}

	std::map<std::string, Channel* >::iterator it = channels.begin();
	
	for ( ; it != channels.end(); it ++ ) {
		it->second->setStatus ( s );
	}
	
	status = s;
}

void ListenerClass::handleConnection(int desc){
	try{
		//Create special socket for protocol version
		//Send it to the waiting line so it can be sent to the correct channel.
		connectionsWaiting.lock ();
		connectionsWaiting.insert ( desc );
		connectionsWaiting.unlock ();
		connectionsWaiting.signal ();
	}catch(LogString e){
		Log("Listener: "+ e);
	}
}

void ListenerClass::Setup(){
	try{
		//select apps.*, users.* from apps left join assoc on assoc.appid = apps.appid left join users on users.userid = assoc.userid
		if ( !appDB.query ( "select * from apps" ) ) {
			throw "Query error occured.";
		}

		
		while ( appDB.hasNext ( ) ) {
			std::vector<std::string> l = appDB.next ( );
			AppDB appMDB;
			appMDB.auth = atoi(l[4].c_str());
			appMDB.host = l[5];
			appMDB.dbname = l[6];
			appMDB.username = l[7];
			appMDB.password = l[8];
			channels.insert ( channels.begin(), std::pair <std::string, Channel* > ( l[2], new Channel ( l[1], appMDB , l[3] , 1000 ) ) ); 			
		}

		chselect = (ChannelSelector**) malloc ( sizeof(ChannelSelector*) * maxSelectors );
		for ( int i = 0; i < maxSelectors; i ++ ) {
			chselect[i] = new ChannelSelector ( &connectionsWaiting, &channels );
		}

		if ( !listenerSocket.bind(port) ) {
			Log ( "Unable to bind port" );
			throw "Unable to bind port";
		}
		if ( !listenerSocket.listen(queue) ) {
			Log ("Unable to listen on port" );
			throw "Unable to listen on port";
		}
		listenerSocket.setNonBlocking ( listenerSocket.getSocket() );
		Log("Listener: Starting");

		
	}catch(const char * e){
		Log("Listener: " + std::string(e));
		std::exit(0);
	}
}

void ListenerClass::Execute(void * arg){
	try{
		Log("Listener: Started");
		while ( status ) {
			//Check for updated scripts and new channels.
			checkForUpdates ( );

			if ( ( incomingFD = listenerSocket.accept() ) > 0) {
				Log("Listener: New Connection");
				handleConnection(incomingFD);
			}
		}
	}catch(const char * e){
		Log("Listener: " + std::string(e));
		std::exit(0);
	}
}

int ListenerClass::checkForUpdates ( ) {
	try {
		time_t time_now = time(NULL);
		if ( difftime ( time_now, lastUpdateCheck ) > elapseUpdateCheck ) {
			lastUpdateCheck = time_now;
			if ( !appDB.query ( "select * from apps" ) ) {
				throw "Query error occured.";
			}

			while ( appDB.hasNext ( ) ) {
				AppDB appMDB;
				std::vector<std::string> l = appDB.next ( );
				std::map<std::string,Channel*>::iterator it;
				if ( ( it = channels.find ( l[2] ) ) != channels.end () ) {
					if ( it->second->currentScript().compare ( l[3] ) != 0 ) {
						Log ( "Listener: Update was found for " + l[2] );
						it->second->updateScript ( l[3] );
					}
				} else {
					appMDB.auth = atoi(l[4].c_str());
					appMDB.host = l[5];
					appMDB.dbname = l[6];
					appMDB.username = l[7];
					appMDB.password = l[8];
					channels.insert ( channels.begin(), std::pair <std::string, Channel* > ( l[2], new Channel ( l[1], appMDB, l[3] , 1000 ) ) ); 
				}			
			}
		}

		return 1;
	} catch ( const char * e ) {
		Log ( "Listener: CHECKFORUPDATES ERROR " + std::string(e)) ;
		return 0;
	}
}

std::string ListenerClass::availableChannels ( ) {
	std::string ret = "";
	std::map<std::string, Channel*>::iterator it = channels.begin();
	for ( ; it != channels.end ();it ++ ) {
		ret += it->second->getName ( ) + "\n";
	}
	return ret;
}

int ListenerClass::usersConnected ( ) {
	int users = 0;
	std::map<std::string, Channel*>::iterator it = channels.begin();
	for ( ; it != channels.end ();it ++ ) {
		users += it->second->usersConnected ( );
	}
	return users;
}
