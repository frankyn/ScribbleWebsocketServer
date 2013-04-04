#include "ConnectionsWaiting.h"

ConnectionsWaiting::ConnectionsWaiting() {
	
}

ConnectionsWaiting::~ConnectionsWaiting() {
	connectionsWaiting.clear();
}

void ConnectionsWaiting::lock ( ) {
	pad.lock();
}

void ConnectionsWaiting::unlock ( ) {
	pad.unlock();
}

void ConnectionsWaiting::insert ( int conn ) {
	connectionsWaiting.push_back ( conn );
}

int ConnectionsWaiting::get ( ) {
	int desc = connectionsWaiting.front ( );
	connectionsWaiting.pop_front ( );
	return desc;
}	

void ConnectionsWaiting::signal ( ) {
	sem.post();
}

int ConnectionsWaiting::wait ( ) {
	return sem.timedWait( 0, 25 );
}
