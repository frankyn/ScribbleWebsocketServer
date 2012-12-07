#include <string>
#include <sqlite3.h>

#ifndef SQLITE_HEADER
#define SQLITE_HEADER
class SQLite { 
	public:
		SQLite ( );
		~SQLite ( );
		
		bool connect ( std::string );
		void disconnect ( );
		std::string query ( std::string );
		int update ( std::string );

	private:
		sqlite3_stmt *statement;
		sqlite3 * dbfile;
		bool isOpenDB;

};

#endif
