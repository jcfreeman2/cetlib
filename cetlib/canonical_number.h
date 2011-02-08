#ifndef CETLIB_CANONICAL_NUMBER_H
#define CETLIB_CANONICAL_NUMBER_H

// ======================================================================
//
// canonical_number: transform a number string into a canonical form
//
// ======================================================================

#include <string>

namespace cet {

  bool
    canonical_number( std::string const & value
                    , std::string       & result
                    );

}

// ======================================================================

#endif
