#include "DBMongo.h"

DBMongo::DBMongo ( ) {

}

DBMongo::~DBMongo ( ) {

}

int DBMongo::connect ( const std::string host ) {
	try {
		connection.connect ( host.c_str() );
		return 1;
	} catch ( const mongo::DBException &e ) {
	    std::cout << "DBMongo Error: " << e.what() << std::endl;
	    return 0;
    }
}
