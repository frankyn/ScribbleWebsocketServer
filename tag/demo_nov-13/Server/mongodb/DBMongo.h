#include <cstdlib>
#include <iostream>
#include "mongo/client/dbclient.h"

#ifndef DBMONGO_HEADER
#define DBMONGO_HEADER
class DBMongo {
	public:
		DBMongo ();
		~DBMongo ();

		int connect ( const std::string );
	private:
		mongo::DBClientConnection connection;
};


#endif