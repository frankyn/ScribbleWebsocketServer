#include "SQLite.h"


SQLite::SQLite ( ) {
	isOpenDB = false;
}

SQLite::~SQLite ( ) {
	
}

bool SQLite::connect ( std::string db ) {

    if ( sqlite3_open(db.c_str(), &dbfile) == SQLITE_OK )
    {
        isOpenDB = true;
        return true;
    }         

    return false;

}

void SQLite::disconnect ( ) {
    if ( isOpenDB ) 
    {
        sqlite3_close(dbfile);
    }
}

int SQLite::update ( std::string q ) {
	int result;    
        if(sqlite3_prepare(dbfile,q.c_str(),-1,&statement,0)==SQLITE_OK)
        {
            int res=sqlite3_step(statement);
            result=res;
            sqlite3_finalize(statement);
        }
	return result;
}

std::string SQLite::query ( std::string q ) { 
    std::string returnVal;
    if ( sqlite3_prepare(dbfile, q.c_str() , -1, &statement, 0 ) == SQLITE_OK ) 
    {
        int ctotal = sqlite3_column_count(statement);
        int res = 0;

        res = sqlite3_step(statement);         
        if ( res == SQLITE_ROW ) 
        {
            for ( int i = 0; i < ctotal; i++ ) 
            {
                returnVal += (char*)sqlite3_column_text(statement, i);
            }
        } 
    }
    return returnVal;
}
