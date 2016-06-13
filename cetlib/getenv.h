#ifndef CETLIB_GETENV_H
#define CETLIB_GETENV_H

// ======================================================================
//
// getenv: Obtain value of environment variable; throw if none such
//
// ======================================================================

#include <string>

namespace cet {
  std::string getenv(std::string const& name);
}

// ======================================================================

#endif

// Local variables:
// mode: c++
// End:
