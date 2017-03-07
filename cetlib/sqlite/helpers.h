#ifndef cetlib_sqlite_helpers_h
#define cetlib_sqlite_helpers_h

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

namespace cet {
  namespace sqlite {

    sqlite3* openDatabaseConnection(std::string const& filename);

    bool hasTable(sqlite3* db, std::string const& tablename);
    bool hasTableWithSchema(sqlite3* db, std::string const& tablename, std::string expectedSchema);
    unsigned nrows(sqlite3* db, std::string const& tablename);

    void delete_from(sqlite3* db, std::string const& tablename);
    void drop_table(sqlite3* db, std::string const& tablename);
    void drop_table_if_exists(sqlite3* db, std::string const& tablename);

    template <typename... Args>
    void createTableIfNeeded(sqlite3* db,
                             bool const delete_contents,
                             std::string const& tablename,
                             permissive_column<Args> const&... cols)
    {
      auto const& sqlddl = detail::create_table_ddl(tablename, cols...);
      if (hasTableWithSchema(db, tablename, sqlddl)) {
        if (delete_contents) {
          delete_from(db, tablename); // Prefer drop_table, but failure-to-prepare exception ends up being thrown.
        }
      }
      else {
        exec(db, sqlddl);
      }
    }

  } //namespace sqlite
} //namespace cet

#endif /* cetlib_sqlite_helpers_h */

// Local Variables:
// mode: c++
// End:
