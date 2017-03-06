#ifndef cetlib_sqlite_select_h
#define cetlib_sqlite_select_h

#include "cetlib/sqlite/detail/get_result.h"
#include "cetlib/sqlite/query_result.h"

#include <string>

#include "sqlite3.h"

namespace cet {
  namespace sqlite {

    template <typename... Args>
    query_result<Args...> query(sqlite3* db, std::string const& ddl)
    {
      query_result<Args...> res;
      char* errmsg {nullptr};
      if (sqlite3_exec(db, ddl.c_str(), detail::get_result<Args...>, &res, &errmsg) != SQLITE_OK) {
        std::string msg{errmsg};
        sqlite3_free(errmsg);
        throw sqlite::Exception(sqlite::errors::SQLExecutionError, msg);
      }
      return res;
    }

    struct SelectStmt {
      SelectStmt(std::string&& ddl, sqlite3* const db) : ddl_{std::move(ddl)}, db_{db} {}
      std::string ddl_;
      sqlite3* db_;

      auto where(std::string const& cond) &&
      {
        ddl_ += " where ";
        ddl_ += cond;
        return SelectStmt{std::move(ddl_), db_};
      }

      auto order_by(std::string const& column, std::string const& clause = {}) &&
      {
        ddl_ += " order by ";
        ddl_ += column;
        ddl_ += " "+clause;
        return SelectStmt{std::move(ddl_), db_};
      }

      auto limit(int const num) &&
      {
        ddl_ += " limit ";
        ddl_ += std::to_string(num);
        return SelectStmt{std::move(ddl_), db_};
      }
    };

    struct IncompleteSelectStmt {

      IncompleteSelectStmt(std::string&& ddl) : ddl_{std::move(ddl)} {}

      auto from(sqlite3* const db, std::string const& tablename) &&
      {
        ddl_ += " from ";
        ddl_ += tablename;
        return SelectStmt{std::move(ddl_), db};
      }
      std::string ddl_;

    };

    namespace detail {
      inline std::string concatenate() { return ""; }

      template <typename H, typename... T>
      std::string concatenate(H const& h, T const&... t)
      {
        return sizeof...(t) != 0u ? std::string{h}+","+concatenate(t...) : std::string{h};
      }
    }

    template <typename... T>
    auto select(T const&... t)
    {
      std::string result {"select "+detail::concatenate(t...)};
      return IncompleteSelectStmt{std::move(result)};
    }

    template <typename... T>
    auto select_distinct(T const&... t)
    {
      std::string result {"select distinct "+detail::concatenate(t...)};
      return IncompleteSelectStmt{std::move(result)};
    }

    template <typename... Args>
    void operator<<(query_result<Args...>& r, SelectStmt const& cq)
    {
      r = query<Args...>(cq.db_, cq.ddl_+";");
    }

  } // sqlite
} // cet

#endif /* cetlib_sqlite_select_h */

// Local variables:
// mode: c++
// End:
