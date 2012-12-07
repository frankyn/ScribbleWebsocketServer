#include <iostream>
#include <cassert>

#include <vector>
#include "SQLite.h"

int main ( ) {
	SQLite sql;
	if ( sql.connect ( "hello.db" ) ) {
		sql.update ( "create table tbl1 (one varchar(10), two smallint);" );
		sql.update ( "insert into tbl1 (one, two) VALUES ( 'test', 10 )" );
		if ( sql.query ( "select * from tbl1 where one='test'" ) ) {
			while ( sql.hasNext ( ) ) {
				std::vector<std::string> l = sql.next ( );
				assert( l.size() == 2 );
			}
		} else {
			std::cout<<"Error"<<std::endl;
		}
	}
};
