#ifndef CETLIB_CANONICAL_STRING_H
#define CETLIB_CANONICAL_STRING_H

// ======================================================================
//
// canonical_string: Transform a string into a canonical form
//
// ======================================================================

#include <string>

namespace cet {

  std::string
    escape( std::string const & str );

  std::string
    unescape( std::string const & str );

  bool
    canonical_string( std::string const & str
                    , std::string       & result
                    );

}

// ======================================================================

#endif
