#include <iostream>
#include <cassert>

#include <vector>
#include "MySQL.h"

int main ( ) {
	/* Create MySQL object */
	MySQL sql;
	
	/* Open Database */
	std::cout << "Opening hello.db" << std::endl;
	assert ( sql.connect ( "localhost", "root", "blue23", "test" ) == true );
	std::cout << "Opening was successful" << std::endl;	

	/* Create a table */		
	std::cout << "Creating table tbl1 (one int, two int)" << std::endl;
	assert ( sql.exec ( "create table tbl1 (one int, two int);" ) == 1 );
	std::cout << "Created table tbl1" << std::endl;
	
	/* Insert values */
	std::cout << "Insert values ( 10, 10 ) into tbl1" << std::endl; 
	assert ( sql.exec ( "insert into tbl1 (one, two) VALUES ( 10, 10 )" ) == 1 );
	std::cout << "Inserted values successfully" << std::endl;

	/* Query values */
	std::cout << "Query values to make sure of integraty" << std::endl;
	assert ( sql.query ( "select * from tbl1" ) == 1);
	std::cout << "Query was successful" << std::endl;
	
	/* Integraty of values */
	std::cout << "Checking to see if query returned results" << std::endl;
	assert ( sql.hasNext ( ) );
	std::cout << "Results were returned" << std::endl;

	/* Check to see that we get a total of two columns ( one, two ) from query */
	std::cout << "Checking to see if a record has two entries ( one, two )" << std::endl;
	std::vector<std::string> l = sql.next ( );
	assert( l.size() == 2 );
	std::cout << "Integraty was achieved for record count" << std::endl;
	
	/* Check Integraty of inserted row ( 10, 10 ) */
	std::cout << "Checking to see one = 10" << std::endl;
	assert( l[0].compare ( "10" ) == 0 );
	std::cout << "Checking to see two = 10" << std::endl; 
	assert( l[1].compare ( "10" ) == 0 );
	
	/* Clear out values */
	std::cout << "Clearing values from database" << std::endl;
	assert ( sql.query ( "delete from tbl1" ) == 1 );
	std::cout << "Database values were cleared" << std::endl;
	
	/* Remove table */
	std::cout << "Removing table tbl1 from database" << std::endl;
	assert ( sql.query ( "DROP TABLE IF EXISTS tbl1" ) == 1);
	std::cout << "Table tbl1 was removed from database" << std::endl;

	/* Close database */
	std::cout << "Database attempting to close" << std::endl;
	sql.disconnect ();
	std::cout << "Database closed" << std::endl;
	
	/* Closing statements */
	std::cout << "Finished unit testing :D" << std::endl;
	return 1;	
};
