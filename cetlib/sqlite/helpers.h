#ifndef cetlib_Ntuple_sqlite_helpers_h
#define cetlib_Ntuple_sqlite_helpers_h

// ====================================================================
//
// sqlite helpers
//
// These are general utilities for interacting with a database and any
// tables therein.
//
// ====================================================================

#include "cetlib/sqlite/Exception.h"
#include "cetlib/sqlite/column.h"
#include "cetlib/sqlite/create_table.h"
#include "cetlib/sqlite/exec.h"
#include "cetlib/sqlite/select.h"

#include "sqlite3.h"

#include <string>

using namespace std::string_literals;

namespace sqlite {

  bool hasTable(sqlite3* db, std::string const& tablename);
  bool hasTableWithSchema(sqlite3* db, std::string const& tablename, std::string const& expectedSchema);
  sqlite3* openDatabaseFile(std::string const& filename);
  void     deleteTable(sqlite3* db, std::string const& tablename);
  void     dropTable  (sqlite3* db, std::string const& tablename);
  unsigned nrows      (sqlite3* db, std::string const& tablename);

  template <typename... ARGS>
  void createTableIfNeeded(sqlite3* db,
                           sqlite3_int64& rowid,
                           bool const delete_contents,
                           std::string const& tname,
                           column<ARGS> const&... cols)
  {
    auto const& sqlddl = detail::create_table_ddl(tname, cols...);
    if (!hasTableWithSchema(db, tname, sqlddl)) {
      exec(db, sqlddl);
    }
    else {
      if (delete_contents) {
        deleteTable(db, tname);
      }
      rowid = nrows(db, tname);
    }
  }

} //namespace sqlite

#endif /* cetlib_Ntuple_sqlite_helpers_h */

// Local Variables:
// mode: c++
// End:
