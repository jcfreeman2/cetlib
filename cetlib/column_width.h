#ifndef CETLIB_COLUMN_WIDTH_H
#define CETLIB_COLUMN_WIDTH_H

// ======================================================================
//
// column_width: Obtain the length of the longest in a vector of strings
//
// ======================================================================

#include <string>
#include <vector>

namespace cet {
  std::string::size_type column_width(std::vector<std::string> const&);
}

#endif

// Local variables:
// mode: c++
// End:
