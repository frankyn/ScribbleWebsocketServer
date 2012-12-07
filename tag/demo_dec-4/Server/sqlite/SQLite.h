#include <iostream>
#include <string>
#include <vector>
#include <sqlite3.h>
#include "../common/Logger.h"
#ifndef SQLITE_HEADER
#define SQLITE_HEADER
class SQLite { 
	public:
		SQLite ( );
		~SQLite ( );
		
		void log ( std::string );
		bool connect ( std::string );
		void disconnect ( );

		int query ( std::string );
		int update ( std::string );
		int hasNext ( );

		std::vector<std::string> next ( );


	private:
		int hasNextRow;
		sqlite3_stmt *statement;
		sqlite3 * dbfile;
		bool isOpenDB;

};

#endif
