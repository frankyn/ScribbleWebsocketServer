#include "MySQL.h"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>

// int main(int argc, char **argv) {
//     /* Get ENV variables */
//     const char *test_host = std::getenv("TEST_HOST");
//     assert(test_host != NULL);
//
//     const char *test_user = std::getenv("TEST_USER");
//     assert(test_user != NULL);
//
//     const char *test_database = std::getenv("TEST_DATABASE");
//     assert(test_database != NULL);
//
//     const char *test_pass = std::getenv("TEST_PASS");
//
//     /* Open Database */
//     std::cout << "Connecting to " << test_host <<
//               " using " << test_database << " database" << std::endl;
//
//     /* Create MySQL object */
//     MySQL sql(test_host, test_user, test_pass, test_database);
//
//     std::cout << "Opening was successful" << std::endl;
//
//     /* Create a table */
//     std::cout << "Creating table tbl1 (one int, two int)" << std::endl;
//     assert(sql.exec("create table tbl1 (one int, two int);") == 1);
//     std::cout << "Created table tbl1" << std::endl;
//
//     /* Insert values */
//     std::cout << "Insert values ( 10, 10 ) into tbl1" << std::endl;
//     assert(sql.exec("insert into tbl1 (one, two) VALUES ( 10, 10 )") == 1);
//     std::cout << "Inserted values successfully" << std::endl;
//
//     /* Query values */
//     std::cout << "Query values to make sure of integraty" << std::endl;
//     assert(sql.query("select * from tbl1") == 1);
//     std::cout << "Query was successful" << std::endl;
//
//     /* Integraty of values */
//     std::cout << "Check query returned results" << std::endl;
//     assert(sql.hasNext());
//     std::cout << "Results were returned" << std::endl;
//
//     /* Check to see that we get a total of two columns ( one, two ) from
//     query */ std::cout << "Check a record has two entries ( one, two )" <<
//     std::endl; std::vector <std::string> row = sql.next(); assert(row.size()
//     == 2); std::cout << "Integraty was achieved for record count" <<
//     std::endl;
//
//     /* Check Integraty of inserted row ( 10, 10 ) */
//     std::cout << "Checking to see one = 10" << std::endl;
//     assert(row[0].compare("10") == 0);
//     std::cout << "Checking to see two = 10" << std::endl;
//     assert(row[1].compare("10") == 0);
//
//     /* Clear out values */
//     std::cout << "Clearing values from database" << std::endl;
//     assert(sql.query("delete from tbl1") == 1);
//     std::cout << "Database values were cleared" << std::endl;
//
//     /* Remove table */
//     std::cout << "Removing table tbl1 from database" << std::endl;
//     assert(sql.query("DROP TABLE IF EXISTS tbl1") == 1);
//     std::cout << "Table tbl1 was removed from database" << std::endl;
//
//     /* Close database */
//     std::cout << "Database attempting to close" << std::endl;
//     sql.disconnect();
//     std::cout << "Database closed" << std::endl;
//
//     /* Closing statements */
//     std::cout << "Finished unit testing :D" << std::endl;
//     return 0;
// };
