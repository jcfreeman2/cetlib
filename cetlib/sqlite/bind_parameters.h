#ifndef cetlib_Ntuple_detail_sqlite_bind_parameter_h
#define cetlib_Ntuple_detail_sqlite_bind_parameter_h

#include "cetlib/sqlite/Exception.h"
#include "sqlite3.h"

#include <tuple>

namespace cet {
  namespace sqlite {
    namespace detail {

      inline
      void bind_one_parameter(sqlite3_stmt* s, std::size_t const idx, double const v)
      {
        int const rc = sqlite3_bind_double(s, idx, v);
        if (rc != SQLITE_OK)
          throw sqlite::Exception{sqlite::errors::SQLExecutionError, "Failed to bind double " + std::to_string(rc)};
      }

      inline
      void bind_one_parameter(sqlite3_stmt* s, std::size_t const idx, int const v)
      {
        int const rc = sqlite3_bind_int(s, idx, v);
        if (rc != SQLITE_OK)
          throw sqlite::Exception{sqlite::errors::SQLExecutionError, "Failed to bind int " + std::to_string(rc)};
      }

      inline
      void bind_one_parameter(sqlite3_stmt* s, std::size_t const idx, std::uint32_t const v)
      {
        int const rc = sqlite3_bind_int64(s, idx, v);
        if (rc != SQLITE_OK)
          throw sqlite::Exception{sqlite::errors::SQLExecutionError, "Failed to bind int " + std::to_string(rc)};
      }

      inline
      void bind_one_parameter(sqlite3_stmt* s, std::size_t const idx, sqlite_int64 const v)
      {
        int const rc = sqlite3_bind_int64(s, idx, v);
        if (rc != SQLITE_OK)
          throw sqlite::Exception{sqlite::errors::SQLExecutionError, "Failed to bind int64 " + std::to_string(rc)};
      }

      inline
      void bind_one_parameter(sqlite3_stmt* s, std::size_t const idx, std::string const& v)
      {
        int const rc = sqlite3_bind_text(s, idx, v.c_str(), v.size(), nullptr);
        if (rc != SQLITE_OK)
          throw sqlite::Exception{sqlite::errors::SQLExecutionError, "Failed to bind text " + std::to_string(rc)};
      }

      inline
      void bind_one_null(sqlite3_stmt* s, std::size_t const idx)
      {
        int const rc = sqlite3_bind_null(s, idx);
        if (rc != SQLITE_OK)
          throw sqlite::Exception{sqlite::errors::SQLExecutionError, "Failed to bind text " + std::to_string(rc)};
      }

      template <class TUP, size_t N>
      struct bind_parameters
      {
        static void bind(sqlite3_stmt* s, TUP const& t)
        {
          bind_parameters<TUP, N-1>::bind(s, t);
          if (auto& param = std::get<N-1>(t))
            bind_one_parameter(s, N, *param);
          else
            bind_one_null(s,N);
        }
      };

      template <class TUP>
      struct bind_parameters<TUP, 1>
      {
        static void bind(sqlite3_stmt* s, TUP const& t)
        {
          if (auto& param = std::get<0>(t))
            bind_one_parameter(s, 1, *param);
          else
            bind_one_null(s,1);
        }
      };

    } // detail
  } // sqlite
} // cet

#endif /* cetlib_Ntuple_sqlite_bind_parameters_h */

// Local Variables:
// mode: c++
// End:
