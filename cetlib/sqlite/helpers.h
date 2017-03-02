#ifndef cetlib_Ntuple_sqlite_helpers_h
#define cetlib_Ntuple_sqlite_helpers_h

// =======================================================
//
// sqlite helpers
//
// =======================================================

#include <array>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

#include "cetlib/sqlite/Exception.h"
#include "cetlib/sqlite/column.h"
#include "cetlib/sqlite/create_table.h"
#include "cetlib/sqlite/exec.h"
#include "cetlib/sqlite/query_result.h"
#include "cetlib/sqlite/select.h"
#include "cetlib/sqlite/stringstream.h"

#include "sqlite3.h"

using namespace std::string_literals;

namespace sqlite {

  namespace detail {

    template <typename COL_PACK, std::size_t... I>
    std::string createTable_ddl(std::string const& tname,
                                COL_PACK const& cols,
                                std::index_sequence<I...>)
    {
      std::string ddl {"CREATE TABLE "s + tname + " ( "};
      ddl += columns(std::get<I>(cols)...);
      ddl += " )";
      return ddl;
    }

    bool hasTable (sqlite3* db, std::string const& name, std::string const& sqlddl);

  } // namespace detail

  //=====================================================================
  // General db functions

  sqlite3* openDatabaseFile(std::string const& filename);
  void     deleteTable(sqlite3* db, std::string const& tname);
  void     dropTable  (sqlite3* db, std::string const& tname);

  unsigned nrows (sqlite3* db, std::string const& tname);

  template <typename... ARGS>
  void createTableIfNeeded(sqlite3* db,
                           sqlite3_int64& rowid,
                           std::string const& tname,
                           column_pack<ARGS...> const& cols,
                           bool const delete_contents)
  {
    std::string const& sqlddl = detail::createTable_ddl(tname, cols, std::index_sequence_for<ARGS...>());
    if (!detail::hasTable(db, tname, sqlddl)) {
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
