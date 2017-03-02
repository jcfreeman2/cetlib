#ifndef cetlib_Ntuple_sqlite_query_result_h
#define cetlib_Ntuple_sqlite_query_result_h

// =======================================================
//
// sqlite query_result
//
// =======================================================

#include <sstream>
#include <vector>

#include "cetlib/Ntuple/Exception.h"
#include "cetlib/container_algorithms.h"
#include "cetlib/sqlite/stringstream.h"

namespace sqlite {

  struct query_result {
    std::vector<std::string> columns;
    std::vector<sqlite::stringstream> data;
    bool empty() const { return data.empty(); }

    auto begin() { return data.begin(); }
    auto end() { return data.end(); }

    explicit operator bool() const { return empty(); }

    template <typename T>
    query_result& operator>>(T& t) {
      if (!data.empty()) {
        data[0] >> t;
      }
      return *this;
    }

    template <typename T>
    query_result& operator>>(std::vector<T>& vt)
    {
      std::vector<T> tmp;
      cet::transform_all(data, std::back_inserter(tmp),
                         [](auto& d){ T t; d >> t; return t; });
      std::swap(vt, tmp);
      data.clear();
      return *this;
    }

  };

  inline query_result& throw_if_empty(query_result& r)
  {
    if (r.empty())
      throw sqlite::Exception{sqlite::errors::SQLExecutionError} << "SQL query failed.";
    return r;
  }

  std::ostream& operator<<(std::ostream&, query_result const&);

} //namespace sqlite

#endif /* cetlib_Ntuple_sqlite_query_result_h */

// Local Variables:
// mode: c++
// End:
