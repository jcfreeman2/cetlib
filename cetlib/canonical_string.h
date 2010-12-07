// ======================================================================
//
// canonical_string
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
