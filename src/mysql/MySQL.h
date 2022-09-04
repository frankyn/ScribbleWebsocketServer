#ifndef MYSQL_HEADER
#define MYSQL_HEADER
#include "../common/Logger.h"
#include <iostream>
#include <mysqlx/xdevapi.h>
#include <sstream>
#include <string>
#include <vector>

class MySQL {
public:
  MySQL(std::string host, std::string username, std::string password,
        std::string database);

  ~MySQL();

  void log(std::string);

  void disconnect();

  int query(std::string);

  int exec(std::string); // alias for query
  int update(std::string);

  int hasNext();

  std::vector<std::string> next();

private:
  // int hasNextRow;
  // int affectedRows;
  ::mysqlx::Session session;
  bool isOpenDB;

  // MYSQL connectionID;
  // MYSQL_RES *result;
  // MYSQL_FIELD *field;

  std::vector<std::vector<std::string>> tableResults;
};

#endif
