#ifndef cetlib_sqlite_create_table_h
#define cetlib_sqlite_create_table_h

// =======================================================
//
// sqlite helpers
//
// =======================================================

#include "sqlite3.h"
#include "cetlib/sqlite/Exception.h"
#include "cetlib/sqlite/column.h"
#include "cetlib/sqlite/exec.h"
#include "cetlib/sqlite/select.h"

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

      inline std::string create_table(std::string const& tablename)
      {
        return "CREATE TABLE "s+tablename;
      }

      template <typename... Cols>
      std::string create_table_ddl(std::string const& tablename, Cols const&... cols)
      {
        std::string ddl {create_table(tablename)};
        ddl += "( ";
        ddl += columns(cols...);
        ddl += " )";
        return ddl;
      }

      inline std::string create_table_as_ddl(std::string const& tablename, SelectStmt const& stmt)
      {
        std::string ddl {create_table(tablename)};
        ddl += " AS ";
        ddl += stmt.ddl_;
        return ddl;
      }

    } // detail

    template <typename... Cols>
    void create_table(sqlite3* const db,
                      std::string const& tablename,
                      Cols const&... cols)
    {
      auto const& ddl = detail::create_table_ddl(tablename, cols...);
      sqlite::exec(db, ddl);
    }

    inline
    void create_table_as(std::string const& tablename,
                         SelectStmt const& stmt)
    {
      std::string ddl {detail::create_table_as_ddl(tablename, stmt)};
      sqlite::exec(stmt.db_, ddl);
    }

  } // sqlite
} // cet

#endif /* cetlib_sqlite_create_table_h */

// Local Variables:
// mode: c++
// End:
