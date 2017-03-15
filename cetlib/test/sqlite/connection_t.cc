#include "boost/filesystem.hpp"
#include "cetlib/SimultaneousFunctionSpawner.h"
#include "cetlib/sqlite/ConnectionFactory.h"
#include "cetlib/sqlite/Ntuple.h"
#include "cetlib/sqlite/create_table.h"

#include <cassert>
#include <iostream>

using namespace std;
using namespace cet::sqlite;
namespace bfs = boost::filesystem;

int main()
  try {

    ConnectionFactory cf;

    // Simple connection to "a.db"
    {
      std::string const f {"a.db"};
      auto c = cf.make(f);
      create_table(c, "onlyOne", column<int>{"numbers"});
      bfs::path const p {f};
      assert(bfs::exists(p));
    }

    // Simple connection to in-memory database
    {
      auto c = cf.make(":memory:");
      create_table(c, "onlyOne", column<int>{"numbers"});
      query_result<int> r;
      r << select("*").from(c, "onlyOne");
      assert(r.empty());
    }

    // Separate connections to "b.db" and "c.db"
    {
      std::string const f1 {"b.db"};
      std::string const f2 {"c.db"};
      auto c1 = cf.make(f1);
      auto c2 = cf.make(f2);
      create_table(c1, "separate", column<int>{"numbers"});
      create_table(c2, "separate", column<int>{"numbers"});
      bfs::path const p1 {f1};
      bfs::path const p2 {f2};
      assert(bfs::exists(p1));
      assert(bfs::exists(p2));
    }

    auto test_colliding_tables = [&cf](std::string const& dbname) {
      auto c1 = cf.make(dbname);
      auto c2 = cf.make(dbname);
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

    // Test concurrent creation of connections to the same database.
    {
      ConnectionFactory sharedFactory;
      std::string const f {"e.db"};
      std::vector<std::function<void()>> const tasks (10, [&sharedFactory,&f]{ auto c = sharedFactory.make(f); });
      cet::SimultaneousFunctionSpawner launch {tasks};
      bfs::path const p {f};
      assert(bfs::exists(p));
    }
  }
  catch(exception const& e) {
    cerr << e.what() << '\n';
    return 1;
  }
  catch(...) {
    cerr << "Caught unknown exception.\n";
    return 2;
  }
