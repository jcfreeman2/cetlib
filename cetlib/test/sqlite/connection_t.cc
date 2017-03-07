#include "boost/filesystem.hpp"
#include "cetlib/sqlite/Connection.h"
#include "cetlib/sqlite/create_table.h"

#include <cassert>
#include <iostream>

using namespace std;
using namespace cet::sqlite;
namespace bfs = boost::filesystem;

int main()
  try {
    // Simple connection to "a.db"
    {
      std::string const f {"a.db"};
      Connection c {f};
      create_table(c, "onlyOne", column<int>{"numbers"});
      bfs::path const p {f};
      assert(bfs::exists(p));
    }

    // Simple connection to in-memory database
    {
      Connection c {":memory:"};
      create_table(c, "onlyOne", column<int>{"numbers"});
      query_result<int> r;
      r << select("*").from(c, "onlyOne");
      assert(r.empty());
    }

    // Separate connections to "b.db" and "c.db"
    {
      std::string const f1 {"b.db"};
      std::string const f2 {"c.db"};
      Connection c1 {f1};
      Connection c2 {f2};
      create_table(c1, "separate", column<int>{"numbers"});
      create_table(c2, "separate", column<int>{"numbers"});
      bfs::path const p1 {f1};
      bfs::path const p2 {f2};
      assert(bfs::exists(p1));
      assert(bfs::exists(p2));
    }

    auto test_colliding_tables = [](std::string const& dbname) {
      Connection c1 {dbname};
      Connection c2 {dbname};
      create_table(c1, "colliding", column<int>{"numbers"});
      try {
        create_table(c2, "colliding", column<int>{"numbers"});
      }
      catch(Exception const& e) {
        assert(e.categoryCode() == errors::SQLExecutionError);
        assert(string{e.what()}.find("table colliding already exists") != string::npos);
      }
    };

    // Separate connections to same database "d.db"
    {
      std::string const f {"d.db"};
      test_colliding_tables(f);
      bfs::path const p {f};
      assert(bfs::exists(p));
    }

    // Separate connections to same database in-memory database
    test_colliding_tables(":memory:");
  }
  catch(exception const& e) {
    cerr << e.what() << '\n';
    return 1;
  }
  catch(...) {
    cerr << "Caught unknown exception.\n";
    return 2;
  }
