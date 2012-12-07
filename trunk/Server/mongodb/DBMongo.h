#include <cstdlib>
#include <iostream>
#include <string>
#include <mongo/client/dbclient.h>
#include "../common/Logger.h"

#ifndef DBMONGO_HEADER
#define DBMONGO_HEADER

class DBMongo {
	public:
		DBMongo ();
		~DBMongo ();

		int connect ( const std::string );
		int auth ( const std::string, const std::string, const std::string );
		int useDB ( const std::string );
		int count ( const std::string, const std::string );

		int query ( const std::string, const std::string );
		int hasNext ( );
		std::string next ( );
		
		int update ( const std::string, const std::string, const std::string );
		int remove ( const std::string, const std::string );
		int insert ( const std::string, const std::string );

	private:
		mongo::DBClientConnection conn;
		mongo::auto_ptr<mongo::DBClientCursor> cursor;
		std::string databaseName;

		const std::string getLink ( const std::string );
};


#endif