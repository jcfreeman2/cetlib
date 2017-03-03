#include "cetlib/container_algorithms.h"
#include "cetlib/sqlite/Connection.h"
#include "cetlib/sqlite/column.h"
#include "cetlib/sqlite/create_table.h"
#include "cetlib/sqlite/insert.h"
#include "cetlib/sqlite/query_result.h"
#include "cetlib/sqlite/select.h"

#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;
using namespace cet::sqlite;

int main()
{
  Connection c {":memory:"};
  std::string const name {"numbers"};

  vector<pair<string,int>> const pairs {{"one", 1}, {"five", 5}, {"ten", 10}};

  create_table(c, name, column<string>("key"), column<int>("value"));
  for (auto const& pr : pairs) {
    insert_into(c, name).values(pr.first, pr.second);
  }

  query_result allEntries;
  allEntries << select("*").from(c, name);

  cet::for_all_with_index(allEntries,
                          [&pairs](std::size_t const i, auto& row){
                            std::string key;
                            int value;
                            row >> key >> value;
                            assert(key == pairs[i].first);
                            assert(value == pairs[i].second);
                          });
}
