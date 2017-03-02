#ifndef cetlib_Ntuple_sqlite_insert_impl_h
#define cetlib_Ntuple_sqlite_insert_impl_h

#include <iostream>
#include <string>
#include <ostream>
#include <sstream>
#include <vector>

#include "cetlib/sqlite/exec.h"
#include "sqlite3.h"

namespace sqlite {
  namespace detail {

    inline std::string maybe_quote(char const* s)
    {
      return "\""+std::string{s}+"\"";
    }

    inline std::string maybe_quote(std::string const& s)
    {
      return "\""+s+"\"";
    }

    template <typename T>
    T maybe_quote(T const& t) { return t; }

    inline void values_str_impl(std::ostream&) {}

    template <typename H, typename... T>
    void values_str_impl(std::ostream& os, H const& h, T const&... t)
    {
      if (sizeof...(T) != 0u) {
        os << maybe_quote(h) << ',';
        values_str_impl(os, t...);
      }
      else
        os << h;
    }

    template <typename... Args>
    std::string values_str(Args const&... args)
    {
      std::ostringstream oss;
      values_str_impl(oss, args...);
      return oss.str();
    }

  }

  struct IncompleteInsert {

    IncompleteInsert(sqlite3* const db, std::string&& ddl)
      : db_{db}
      , ddl_{std::move(ddl)}
    {}

    template <typename... T>
    void values(T const&... t) &&
    {
      ddl_ += " values (";
      ddl_ += detail::values_str(t...);
      ddl_ += ");";
      exec(db_, ddl_);
    }

    sqlite3* const db_;
    std::string ddl_;
  };

  template <typename... C>
  auto insert_into(sqlite3* const db, std::string const& tablename)
  {
    std::string result {"insert into "+tablename};
    return IncompleteInsert{db, std::move(result)};
  }

}

#endif /* cetlib_Ntuple_sqlite_insert_impl_h */

// Local variables:
// mode: c++
// End:
