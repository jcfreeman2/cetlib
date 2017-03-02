// =======================================================
//
// sqlite statistics
//
// =======================================================

#include <cmath>

#include "cetlib/sqlite/Exception.h"
#include "cetlib/sqlite/statistics.h"

using namespace std::string_literals;

namespace sqlite {

  double mean(sqlite3* const db, std::string const& tname, std::string const& colname)
  {
    double result {};
    auto r = query(db, "select avg("s+colname+") from " + tname);
    throw_if_empty(r) >> result;
    return result;
  }

  double median(sqlite3* const db, std::string const& tname, std::string const& colname)
  {
    double result {};
    auto r = query(db,
                   "select avg("s+colname+")"+
                   " from (select "+colname+
                   " from "+tname+
                   " order by "+colname+
                   " limit 2 - (select count(*) from " + tname+") % 2"+
                   " offset (select (count(*) - 1) / 2"+
                   " from " + tname+"))");
    throw_if_empty(r) >> result;
    return result;
  }

  double rms(sqlite3* const db, std::string const& tname, std::string const& colname)
  {
    double result {};
    auto r = query(db,
                   "select sum("s+
                   "(" + colname + "-(select avg(" + colname + ") from " + tname +"))" +
                   "*" +
                   "(" + colname + "-(select avg(" + colname + ") from " + tname +"))" +
                   " ) /" +
                   "(count(" + colname +")) from " + tname);
    throw_if_empty(r) >> result;
    return std::sqrt(result);
  }

} // namespace sqlite
