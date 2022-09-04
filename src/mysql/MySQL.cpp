#include "MySQL.h"
using namespace std;
using namespace ::mysqlx;

MySQL::MySQL(std::string host, std::string username, std::string password,
             std::string database)
    : session(SessionSettings(host, username, password, database)) {
  isOpenDB = false;
  {
    RowResult res = session.sql("show variables like 'version'").execute();
    std::stringstream version;

    version << res.fetchOne().get(1).get<std::string>();
    int major_version;
    version >> major_version;

    if (major_version >= 8) {
      isOpenDB = true;
    }
  }
}

MySQL::~MySQL() { disconnect(); }

void MySQL::disconnect() {
  if (isOpenDB) {
    isOpenDB = false;
    session.close();
  }
}

/*Alias for update*/
int MySQL::exec(std::string q) { return update(q); }

int MySQL::update(std::string q) {
  query(q);

  return 1;
}

int MySQL::query(std::string q) {
  session.sql(q);
  /*affectedRows = 0;

  log("QUERY:" + q);

  if (mysql_query(&connectionID, q.c_str()) == 0) {
      log("Query OK.");

      result = mysql_store_result(&connectionID);
      //Result is good
      if (result) {
          unsigned int num_fields, i;
          MYSQL_ROW row;

          num_fields = mysql_num_fields(result);
          while ((row = mysql_fetch_row(result))) {
              std::vector <std::string> val;
              for (i = 0; i < num_fields; i++) {
                  val.push_back(row[i]);
              }
              tableResults.push_back(val);
          }
          mysql_free_result(result);

      } else {
          if (mysql_field_count(&connectionID) == 0) {
              // query does not return data
              // (it was not a SELECT)
              affectedRows = mysql_affected_rows(&connectionID);
          }
          mysql_free_result(result);
      }
      return 1;
  } else {
      log("Query errored.");
      return 0;
  }*/
  return 0;
}

std::vector<std::string> MySQL::next() {
  std::vector<std::string> returnVal;

  // returnVal = tableResults.front();
  // tableResults.erase(tableResults.begin());

  return returnVal;
}

int MySQL::hasNext() { return tableResults.size() > 0 ? 1 : 0; }

void MySQL::log(std::string msg) { Log("MySQL: " + msg); }
