#ifndef CETLIB_COLUMN_WIDTH_H
#define CETLIB_COLUMN_WIDTH_H

// ======================================================================
//
// column_width: obtain the length of the longest in a vector of strings
//
// ======================================================================

#include <algorithm>
#include <string>
#include <vector>

namespace cet {
  std::string::size_type
    column_width( std::vector<std::string> const & );
}

// ----------------------------------------------------------------------

std::string::size_type
  cet::column_width( std::vector<std::string> const &  v  )
{
  std::string::size_type  maxlen = 0;

  // invariant: maxlen is the length of the longest string in v[0..i)
  for( std::vector<std::string>::size_type i = 0
                                         , e = v.size();  i != e;  ++i )
    maxlen = std::max( maxlen, v[i].size() );

  return maxlen;
}  // column_width()

// ======================================================================

#endif
