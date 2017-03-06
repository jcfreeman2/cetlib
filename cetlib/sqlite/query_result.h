#ifndef cetlib_sqlite_query_result_h
#define cetlib_sqlite_query_result_h

// =======================================================
//
// sqlite query_result
//
// =======================================================

#include <sstream>
#include <tuple>
#include <vector>

#include "cetlib/sqlite/Exception.h"
#include "cetlib/container_algorithms.h"

namespace cet {
  namespace sqlite {

    template <typename... Args>
    struct query_result {
      std::vector<std::string> columns;
      std::vector<std::tuple<Args...>> data;
      bool empty() const { return data.empty(); }

      auto begin() { return data.begin(); }
      auto end() { return data.end(); }

      explicit operator bool() const { return !empty(); }
    };

    template <typename... Args>
    inline void throw_if_empty(query_result<Args...>& r)
    {
      if (r.empty()) {
        throw sqlite::Exception{sqlite::errors::SQLExecutionError} << "SQL query failed.";
      }
    }

    template <typename T>
    inline T unique_value(query_result<T> const& r)
    {
      if (r.data.size() != 1ull) {
        throw sqlite::Exception{sqlite::errors::SQLExecutionError}
        << "unique_value expected of non-unique query.";
      }
      return std::get<T>(r.data[0]);
    }

    template <typename... Args>
    std::ostream& operator<<(std::ostream& os, query_result<Args...> const& res)
    {
      using size_t = decltype(res.columns.size());
      auto const ncolumns = res.columns.size();
      for (size_t i{}; i!= ncolumns ; ++i) {
        os << res.columns[i] << ' ';
      }
      os << "\n--------------------------------\n";
      for (auto const& row : res.data) {
        //        os << row.str() << '\n';
      }
      return os;
    }

  } // sqlite
} // cet

#endif /* cetlib_sqlite_query_result_h */

// Local Variables:
// mode: c++
// End:
