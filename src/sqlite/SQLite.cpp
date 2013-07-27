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

/*Alias for update*/
int SQLite::exec ( std:: string q ) {
    update ( q );
}

int SQLite::update ( std::string q ) {
    log ( "QUERY:" + q );
    int result;    
        if(sqlite3_prepare(dbfile,q.c_str(),-1,&statement,0)==SQLITE_OK)
        {
            int res=sqlite3_step(statement);
            result=res;
            sqlite3_finalize(statement);
        }
	return result;
}

int SQLite::query ( std::string q ) { 
    log ( "QUERY:" + q );
    if ( sqlite3_prepare(dbfile, q.c_str() , -1, &statement, 0 ) == SQLITE_OK ) {
        hasNextRow = sqlite3_step(statement);  
        log ( "Query OK." );
        return 1;
    } else {
        log ( "Query errored." );
        return 0;
    }
}

std::vector<std::string> SQLite::next ( ) {
    std::vector<std::string> returnVal;
    int ctotal = sqlite3_column_count(statement);
   
    if ( hasNextRow == SQLITE_ROW ) 
    {
        for ( int i = 0; i < ctotal; i++ ) 
        {
            returnVal.push_back ( (char*)sqlite3_column_text ( statement, i ) );
        }
        hasNextRow = sqlite3_step(statement);  
    } else {
        sqlite3_finalize ( statement );
    }

    return returnVal;
}

int SQLite::hasNext ( ) {
    return hasNextRow==SQLITE_ROW?1:0;
}

void SQLite::log ( std::string msg ) {
    Log ( "SQLite: " + msg );
}
