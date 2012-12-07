#include "DBMongo.h"

int main ( ) {
	DBMongo mongoConnection;
	if ( !mongoConnection.connect ( "localhost" ) ) {
		return -1;
	}

	return 0;
}