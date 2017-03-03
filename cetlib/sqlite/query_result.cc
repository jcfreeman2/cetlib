#include "cetlib/sqlite/query_result.h"
#include <ostream>

std::ostream&
cet::sqlite::operator<<(std::ostream& os, query_result const& res)
{
  using size_t = decltype(res.columns.size());
  auto const ncolumns = res.columns.size();
  for (size_t i{}; i!= ncolumns ; ++i) {
    os << res.columns[i] << ' ';
  }
  os << "\n--------------------------------\n";
  for (auto const& row : res.data) {
    for (size_t i{}; i != ncolumns; ++i) {
      os << row[i] << ' ';
    }
    os << '\n';
  }
  return os;
}
