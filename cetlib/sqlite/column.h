#ifndef cetlib_sqlite_column_h
#define cetlib_sqlite_column_h

// =======================================================
//
// columns
//
// FIXME: EXPLANATION
// =======================================================

#include "cetlib/sqlite/detail/column_constraint.h"

#include <array>
#include <string>
#include <utility>
#include <tuple>

namespace cet {
  namespace sqlite {

    template <size_t N>
    using name_array = std::array<std::string, N>;

    class column_base {
    public:
      column_base(std::string const& n) : name_{n} {}
      auto const& name() const { return name_; }
    private:
      std::string name_;
    };

    // column<T> is a containing struct that knows its C++ type (T)
    // and the sqlite translation (sqlite_type()).  There is no
    // implementation for the general case; the template must be
    // specialized for each supported type.
    template <typename T, typename... Constraints>
    struct column;

    template <typename... Constraints>
    struct column<double, Constraints...> : column_base {
      using column_base::column_base;
      using type = double;
      std::string sqlite_type() const { return " numeric"; }
    };

    template <typename... Constraints>
    struct column<float, Constraints...> : column_base {
      using column_base::column_base;
      using type = float;
      std::string sqlite_type() const { return " numeric"; }
    };

    template <typename... Constraints>
    struct column<int, Constraints...> : column_base {
      using column_base::column_base;
      using type = int;
      std::string sqlite_type() const { return " integer"; }
    };

    template <typename... Constraints>
    struct column<long, Constraints...> : column_base {
      using column_base::column_base;
      using type = long;
      std::string sqlite_type() const { return " integer"; }
    };

    template <typename... Constraints>
    struct column<long long, Constraints...> : column_base {
      using column_base::column_base;
      using type = long long;
      std::string sqlite_type() const { return " integer"; }
    };

    template <typename... Constraints>
    struct column<unsigned int, Constraints...> : column_base {
      using column_base::column_base;
      using type = int;
      std::string sqlite_type() const { return " integer"; }
    };

    template <typename... Constraints>
    struct column<unsigned long, Constraints...> : column_base {
      using column_base::column_base;
      using type = long;
      std::string sqlite_type() const { return " integer"; }
    };

    template <typename... Constraints>
    struct column<unsigned long long, Constraints...> : column_base {
      using column_base::column_base;
      using type = long long;
      std::string sqlite_type() const { return " integer"; }
    };

    template <typename... Constraints>
    struct column<std::string, Constraints...> : column_base {
      using column_base::column_base;
      using type = std::string;
      std::string sqlite_type() const { return " text"; }
    };

    //====================================================================
    template <typename T, typename... Constraints>
    struct permissive_column : column<T, Constraints...> {
      using column<T, Constraints...>::column;
      using element_type = T;
    };

    template <typename T, typename... Constraints>
    struct permissive_column<column<T,Constraints...>> : permissive_column<T, Constraints...> {
      using permissive_column<T, Constraints...>::permissive_column;
    };

  } // sqlite
} // cet

#endif /* cetlib_sqlite_column_h */

// Local Variables:
// mode: c++
// End:
