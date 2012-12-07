#include "DBMongo.h"
DBMongo::DBMongo ( ) {

}

DBMongo::~DBMongo ( ) {

}

int DBMongo::connect ( const std::string host ) {
	try {
		conn.connect ( host );
		return 1;
	} catch ( const mongo::DBException &e ) {
		Log ( "DBMongo: " + std::string(e.what ( )) );
		return 0;
	}
}

int DBMongo::auth ( const std::string dbname, const std::string username, const std::string password ) {
	try {
		std::string t;
		if (!conn.auth ( dbname, username, password, t, true ) ) {
			Log ( "DBMong: Unable to authenticate." );
		}
		return 1;
	} catch ( const char * e ) {
		Log ( "DBMongo: " + std::string ( e ) );
		return 0;
	}
}

int DBMongo::useDB ( const std::string dbname ) {
	Log ( "DBMongo: USING " + dbname + "." );
	databaseName = dbname;

	return 1;
}

int DBMongo::count ( const std::string table, const std::string json ) {
	try {
		Log ( "DBMongo: COUNT " + table + " WHERE " + json   );
		return conn.count ( getLink ( table ).c_str(), mongo::fromjson ( json.c_str()) );
	} catch ( const mongo::DBException &e ) {
		Log ( "DBMongo: " + std::string ( e.what () ) );
		return 0;
	} catch ( const char * e ) {
		Log ( "DBMongo: " + std::string ( e ) );
		return 0;
	}
}

int DBMongo::query ( const std::string table, const std::string json ) {
	try {
		if ( databaseName.empty ( ) ) {
			throw "Database not selected";
		}
		Log ( "DBMongo: QUERY " + table + " WHERE " + json  );
		cursor = conn.query ( getLink ( table ).c_str(), mongo::fromjson ( json.c_str()) );
		//add in vector std and create a vector idea like sqlite wrapper.
		return 1;
	} catch ( const mongo::DBException &e ) {
		Log ( "DBMongo: " + std::string(e.what ( )) );
		return 0;
	} catch ( const char * e ) {
		Log ( "DBMongo: " + std::string(e) );
		return 0;
	}
}
int DBMongo::hasNext ( ) {
	try { 
		if ( cursor->more ( ) ) {
			return 1;
		}
	} catch ( const mongo::DBException &e ) {
		Log ( "DBMongo: " + std::string ( e.what ( ) ) );
	} catch ( const char * e ) {
		Log ( "DBMongo: " + std::string ( e ) );
	}
	return 0;
}
std::string DBMongo::next ( ) {
	std::string record;
	try {
		if ( cursor->more ( ) ) {
			mongo::BSONObj p = cursor->next ( );
			Log ( "DBMongo: NEXT " + p.toString () );
			record = p.jsonString ();
		}
	} catch ( const mongo::DBException &e ) {
		Log ( "DBMongo: " + std::string ( e.what ( ) ) );
	} catch ( const char * e ) {
		Log ( "DBMongo: " + std::string ( e ) );
	}
	return record;
}

int DBMongo::update ( const std::string table, const std::string where, const std::string json ) {
	try {
		Log ( "DBMongo: UPDATE " + table + " WHERE " + where + " TO " + json );
		conn.update ( getLink ( table ).c_str(), mongo::fromjson ( where.c_str()), mongo::fromjson ( json.c_str() ) );
		return 1;
	} catch ( const mongo::DBException &e ) {
		Log ( "DBMongo: " + std::string(e.what ( )) );
		return 0;
	}
}

int DBMongo::insert ( const std::string table, const std::string json ) {
	try {
		Log ( "DBMongo: INSERT " + table + " WITH " + json );
		conn.insert ( getLink ( table ).c_str(), mongo::fromjson ( json.c_str() ) );
		return 1;
	} catch ( const mongo::DBException &e ) {
		Log ( "DBMongo: " + std::string(e.what ( )) );
		return 0;
	}
}

int DBMongo::remove ( const std::string table, const std::string json ) {
	try {
		Log ( "DBMongo: REMOVE FROM " + table + " WHERE " + json );
		conn.remove ( getLink ( table ).c_str(), mongo::fromjson ( json.c_str() ) );
		return 1;
	} catch ( const mongo::DBException &e ) {
		Log ( "DBMongo: " + std::string(e.what ( )) );
		return 0;
	}
}

const std::string DBMongo::getLink ( const std::string table ) {
	if ( databaseName.empty () ) {
		throw "Database name wasn't set";
	}
	return std::string ( databaseName+"."+table ).c_str();
}