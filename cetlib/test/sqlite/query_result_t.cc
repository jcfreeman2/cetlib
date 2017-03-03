#include "cetlib/sqlite/DBmanager.h"
#include "cetlib/sqlite/column.h"
#include "cetlib/sqlite/create_table.h"
#include "cetlib/sqlite/insert.h"
#include "cetlib/sqlite/query_result.h"

#include <cassert>
#include <iostream>

using namespace std;
using namespace sqlite;

int main()
{
  DBmanager db {":memory:"};
  std::string const name {"numbers"};

  create_table(db, name, column<string>("key"), column<int>("value"));
  insert_into(db, name).values("one", 1);
  insert_into(db, name).values("five", 5);
  insert_into(db, name).values("ten", 10);

  query_result allEntries;
  allEntries << select("*").from(db, name);
  std::cout << allEntries << '\n';

}
