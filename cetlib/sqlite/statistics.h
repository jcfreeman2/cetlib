#ifndef cetlib_sqlite_statistics_h
#define cetlib_sqlite_statistics_h

// =======================================================
//
// sqlite statistics
//
// =======================================================

#include "cetlib/sqlite/query_result.h"
#include "cetlib/sqlite/select.h"
#include "sqlite3.h"

#include <string>


namespace cet {
  namespace sqlite {

    template <typename T = double>
    T min(sqlite3* const db, std::string const& tname, std::string const& colname)
    {
      auto r = query<T>(db, "select min("+colname+") from " + tname);
      return unique_value(r);
    }

    template <typename T = double>
    T max(sqlite3* const db, std::string const& tname, std::string const& colname)
    {
      auto r = query<T>(db, "select max("+colname+") from " + tname);
      return unique_value(r);
    }

    double mean  (sqlite3* db, std::string const& tname, std::string const& colname);
    double median(sqlite3* db, std::string const& tname, std::string const& colname);
    double rms   (sqlite3* db, std::string const& tname, std::string const& colname);

  } // sqlite
} // cet

#endif /* cetlib_sqlite_statistics_h */

// Local Variables:
// mode: c++
// End:
