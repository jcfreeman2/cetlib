// =======================================================
//
// sqlite statistics
//
// =======================================================

#include <cmath>

#include "cetlib/sqlite/Exception.h"
#include "cetlib/sqlite/statistics.h"

using namespace std::string_literals;

double
cet::sqlite::mean(sqlite3* const db, std::string const& tname, std::string const& colname)
{
  auto r = query<double>(db, "select avg("s+colname+") from " + tname);
  return unique_value(r);
}

double
cet::sqlite::median(sqlite3* const db, std::string const& tname, std::string const& colname)
{
  auto r = query<double>(db,
                         "select avg("s+colname+")"+
                         " from (select "+colname+
                         " from "+tname+
                         " order by "+colname+
                         " limit 2 - (select count(*) from " + tname+") % 2"+
                         " offset (select (count(*) - 1) / 2"+
                         " from " + tname+"))");
  return unique_value(r);
}

double
cet::sqlite::rms(sqlite3* const db, std::string const& tname, std::string const& colname)
{
  auto r = query<double>(db,
                         "select sum("s+
                         "(" + colname + "-(select avg(" + colname + ") from " + tname +"))" +
                         "*" +
                         "(" + colname + "-(select avg(" + colname + ") from " + tname +"))" +
                         " ) /" +
                         "(count(" + colname +")) from " + tname);
  return std::sqrt(unique_value(r));
}
