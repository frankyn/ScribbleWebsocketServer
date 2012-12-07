#include <iostream>
#include <string>
#include "DBMongo.h"

int main ( int argc, char ** argv ) {
	DBMongo mongoConnection;
	
	if ( !mongoConnection.connect ( "localhost" ) ) {
		std::cout<<"Unable to connect"<<std::endl;
		return -1;
	}

	/*
	if ( !mongoConnection.connect ( "ds031567.mongolab.com:31567" ) ) {
		std::cout<<"Unable to connect"<<std::endl;
		return -1;
	}
	if ( !mongoConnection.auth ( "scribble", "frankyn", "blue23" ) ) {
		std::cerr<<"Unable to authenticate"<<std::endl;
		return -1;
	}
	*/
	std::cout<<"Connected"<<std::endl;
	std::cout<<"Selecting 'test' database"<<std::endl;
	mongoConnection.useDB ( "test" );
	
	std::cout<<"Checking if 'test' table is empty before testing: ";
	assert(mongoConnection.count ( "test", "{}" )==0);
	std::cout<<"OK"<<std::endl;

	std::cout<<"Inserting {'name':'adri'}"<<std::endl;
	mongoConnection.insert ( "test", "{'name':'adri'}" );
	
	//Do a check to make sure the insertion was successful we should have 1 record.
	std::cout<<"Checking if 'test' table has 1 entry: ";
	assert(mongoConnection.count ( "test", "{}" )==1);	
	std::cout<<"OK"<<std::endl;

	mongoConnection.query ( "test", "{}" );
	if ( mongoConnection.hasNext () ) {
		std::cout<<"Checking integrety of update: ";
		assert ( mongoConnection.next ().find("adri") != std::string::npos );
		std::cout<<"OK"<<std::endl;
	} else {
		std::cout<<"Unexpected error occured."<<std::endl;
		return -1;
	}

	std::cout<<"Update {'name':'adri'} -> {$set:{'name':'brittany'}}"<<std::endl;
	mongoConnection.update ( "test", "{'name':'adri'}", "{$set:{'name':'brittany'}}" );

	mongoConnection.query ( "test" , "{}" );
	if ( mongoConnection.hasNext () ) {
		std::cout<<"Checking integrety of update: ";
		assert ( mongoConnection.next ().find("brittany") != std::string::npos );
		std::cout<<"OK"<<std::endl;
	} else {
		std::cout<<"Unexpected error occured."<<std::endl;
		return -1;
	}

	std::cout<<"Remove {'name':'brittany'}"<<std::endl;
	mongoConnection.remove ( "test", "{'name':'brittany'}" );
	
	std::cout<<"Checking if 'test' table has no entries: ";
	assert(mongoConnection.count ( "test", "{}" )==0);	
	std::cout<<"OK"<<std::endl;

	return 0;
}

