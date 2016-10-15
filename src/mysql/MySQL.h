#include <iostream>
#include <string>
#include <vector>
#include <mysql/mysql.h>
#include "../common/Logger.h"

#ifndef MYSQL_HEADER
#define MYSQL_HEADER
class MySQL { 
  public:
    MySQL();
    ~MySQL();
    
    void log(std::string);
    bool connect(std::string, std::string, std::string, std::string);
    void disconnect();

    int query(std::string);
    int exec(std::string); //alias for query   
    int update(std::string);
    int hasNext();

    std::vector<std::string> next();

  private:
    int hasNextRow;
    int affectedRows;
    bool isOpenDB;

    MYSQL connectionID;
    MYSQL_RES *result;
    MYSQL_FIELD *field;

    std::vector< std::vector < std::string > > tableResults;
};
#endif

