#ifndef cetlib_Ntuple_create_table_h
#define cetlib_Ntuple_create_table_h

// =======================================================
//
// sqlite helpers
//
// =======================================================

#include "sqlite3.h"
#include "cetlib/sqlite/Exception.h"
#include "cetlib/sqlite/column.h"
#include "cetlib/sqlite/exec.h"

#include <array>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

using namespace std::string_literals;

namespace cet {
  namespace sqlite {

    namespace detail {

      template <typename T>
      std::string column_info(column<T> const& col)
      {
        return col.name() + col.sqlite_type();
      }

      inline std::string columns() { return ""; }

      template <typename H, typename... T>
      inline std::string columns(H const& h, T const&... t)
      {
        return (sizeof...(T) != 0u) ? column_info(h) + ", " + columns(t...) : column_info(h);
      }

      template <typename... Cols>
      std::string create_table_ddl(std::string const& tname, Cols const&... cols)
      {
        std::string ddl {"CREATE TABLE "s + tname + " ("};
        ddl += columns(cols...);
        ddl += " )";
        return ddl;
      }

    } // detail

    template <typename... Cols>
    auto create_table(sqlite3* const db,
                      std::string const& tablename,
                      Cols const&... cols)
    {
      auto const& ddl = detail::create_table_ddl(tablename, cols...);
      sqlite::exec(db, ddl);
      // return CreateTable object?  Call exec during d'tor?
    }

  } // sqlite
} // cet

#endif /* cetlib_Ntuple_sqlite_helpers_h */

// Local Variables:
// mode: c++
// End:
