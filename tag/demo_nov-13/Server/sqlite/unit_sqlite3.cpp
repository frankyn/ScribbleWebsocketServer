#include <iostream>
#include "SQLite.h"

int main ( ) {
	SQLite sql;
	if ( sql.connect ( "hello.db" ) ) {
		std::cout<<"HELLO"<<std::endl;
	}
	
};
