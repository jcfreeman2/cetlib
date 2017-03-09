#include "cetlib/SimultaneousFunctionSpawner.h"
#include "cetlib/sqlite/Connection.h"
#include "cetlib/sqlite/Exception.h"
#include "cetlib/sqlite/Ntuple.h"
#include "cetlib/sqlite/helpers.h"
#include "cetlib/sqlite/select.h"

#include "sqlite3.h"

#include <assert.h>
#include <cstring>
#include <functional>
#include <iostream>
#include <vector>

// We do not use Boost Unit Test here because we want the product into
// which this moves to be independent of Boost.

using namespace cet::sqlite;

void nullptr_build_failure()
{
  std::cout << "start nullptr_build_failure\n";
  sqlite3* db = nullptr;
  try {
    Ntuple<int, column<double>> t2 {db, "table1", {"i", "x"}};
    assert("Ntuple creation failed to throw required exception" == 0);
  }
  catch (Exception const& x) { }
  catch (...) { assert("Ntuple creation throw the wrong type of exception" == 0); }
  std::cout << "end nullptr_build_failure\n";
}

void test_with_new_database(sqlite3* db)
{
  std::cout << "start test_with_new_database\n";
  assert(db);
  Ntuple<double, std::string> xx {db, "xx", {"x", "txt"}};
  std::cout << "end test_with_new_database\n";
}

void test_with_matching_table(sqlite3* db)
{
  std::cout << "start test_with_matching_table\n";
  assert(db);
  Ntuple<double, std::string> xx {db, "xx", {"x", "txt"}};
  std::cout << "end test_with_matching_table\n";
}

template <class ... ARGS>
void test_with_colliding_table(sqlite3* db,
                               std::array<std::string, sizeof...(ARGS)> const& names)
{
  std::cout << "start test_with_colliding_table\n";
  assert(db);
  try {
    Ntuple<ARGS...> xx {db, "xx", names};
    assert("Failed throw for mismatched table" == nullptr);
  }
  catch (Exception const& x) { }
  catch (...) { assert("Threw wrong exception for mismatched table" == nullptr); }
  std::cout << "end test_with_colliding_table\n";
}

int count_rows(void* p, int nrows, char** results, char** cnames)
{
  auto n = static_cast<int*>(p);
  assert(nrows == 1);
  assert(std::strcmp(cnames[0], "count(*)") == 0);
  *n = std::stoi(results[0]);
  return 0;
}

void test_filling_table(sqlite3* db)
{
  std::cout << "start test_filling_table\n";
  assert(db);
  constexpr int nrows {903};
  {
    Ntuple<int, double> nt {db, "zz", {"i", "x"}, false, 100};
    for (int i = 0; i < nrows; ++i) {
      nt.insert(i, 1.5 * i);
    }
  }
  query_result<int> nmatches;
  nmatches << select("count(*)").from(db, "zz");
  // Check that there are 'nrows' rows in the database.
  assert(unique_value(nmatches) == nrows);
  std::cout << "end test_filling_table\n";
}

void test_parallel_filling_table(sqlite3* db)
{
  std::cout << "start test_parallel_filling_table\n";
  assert(db);
  constexpr int nrows_per_thread {100};
  constexpr unsigned nthreads {10};
  std::string const tablename {"zz"};
  {
    Ntuple<int, double> nt {db, tablename, {"i", "x"}, true, 60}; // Force flushing after 60 insertions.
    std::vector<std::function<void()>> tasks;
    for (unsigned i {}; i < nthreads; ++i) {
      tasks.emplace_back([i,&nt]{
          for (unsigned j {}; j < nrows_per_thread; ++j) {
            auto const j1 = j+i*100;
            nt.insert(j1, 1.5*j1);
          }
        });
    }
    cet::SimultaneousFunctionSpawner sfs {tasks};
  }
  query_result<int> nmatches;
  nmatches << select("count(*)").from(db, tablename);
  assert(unique_value(nmatches) == nrows_per_thread*nthreads);
  std::cout << "end test_parallel_filling_table\n";
}

void test_column_constraint(sqlite3* db)
{
  std::cout << "start test_column_constraint\n";
  assert(db);
  Ntuple<column<int, primary_key>, double> nt {db, "u", {"i", "x"}};
  auto const& ref = detail::create_table_ddl("u", column<int, primary_key>{"i"}, column<double>{"x"});
  assert(hasTableWithSchema(db, "u", ref));
  std::cout << "end test_column_constraint\n";
}

void test_file_create()
{
  std::string const filename {"myfile.db"};
  remove(filename.c_str());
  {
    Ntuple<int, double, int> table {filename, "tab1", {"i", "x", "k" }, false, 5};
    for (std::size_t i = 0; i < 103; ++i) {
      table.insert(i, 0.5*i, i*i);
    }
  }
  Connection c {filename};
  query_result<int> cnt;
  cnt << select("count(*)").from(c, "tab1");
  assert(unique_value(cnt) == 103);
}

int main()
  try {
    const char* fname {"no_such_file.db"};
    nullptr_build_failure();
    sqlite3* db = nullptr;
    // If there is a database in the directory, delete it.
    remove(fname);
    // Now make a database we are sure is empty.
    int const status {sqlite3_open_v2(fname,
                                      &db,
                                      SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                                      nullptr)};
    assert(status == SQLITE_OK);
    test_with_new_database(db);
    test_with_matching_table(db);
    test_with_colliding_table<int, double>(db, {"y", "txt"});
    test_with_colliding_table<int, double>(db, {"x", "text"});
    test_with_colliding_table<int, int>(db, {"x", "txt"});
    test_with_colliding_table<int, double, int>(db, {"x", "txt", "z"});
    test_with_colliding_table<int>(db, {"x"});
    test_filling_table(db);
    test_parallel_filling_table(db);
    test_column_constraint(db);
    // Close database.
    sqlite3_close(db);

    test_file_create();
  }
  catch (std::exception const& x) {
    std::cout << x.what() << std::endl;
    return 1;
  }
  catch (...) {
    std::cout << "Unknown exception caught\n";
    return 2;
  }
