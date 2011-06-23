#ifndef cetlib_demangle_h
#define cetlib_demangle_h
////////////////////////////////////////////////////////////////////////
// Call the cross-vendor API to demangle symbols (eg for ROOT dictionary
// errors).
////////////////////////////////////////////////////////////////////////

#include <string>

namespace cet {
  std::string demangle_symbol(std::string const &mangled);
  std::string demangle_message(std::string const &mangled);
  std::string demangle(std::string const &mangled); // Deprecated.
}

#endif /* cetlib_demangle_h */

// Local Variables:
// mode: c++
// End:
