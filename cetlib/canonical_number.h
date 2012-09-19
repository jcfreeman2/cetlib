#ifndef CETLIB_CANONICAL_NUMBER_H
#define CETLIB_CANONICAL_NUMBER_H

// ======================================================================
//
// canonical_number: Transform a number string into a canonical form
//
// "Number string" is defined by the following regular expression:
//      ^[-+]?([0-9]*\.?[0-9]+|[0-9]+\.?[0-9]*)([eE][-+]?[0-9]+)?$
//
// ======================================================================

#include "cpp0x/string"

namespace cet {

  bool
    canonical_number( std::string const & value
                    , std::string       & result
                    );

}

// ======================================================================

#endif
