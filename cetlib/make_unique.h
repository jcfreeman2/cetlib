#ifndef CETLIB_MAKE_UNIQUE_H
#define CETLIB_MAKE_UNIQUE_H

#include <memory>
#include <utility>

#define DEPRECATION_MESSAGE                                     \
  "\n\n"                                                        \
  "cetlib warning: 'cet::make_unique' has been deprecated.\n"   \
  "                Use 'std::make_unique' instead.\n\n"

namespace cet
{
  template <class T, class ... Args>
  [[deprecated(DEPRECATION_MESSAGE)]]
  auto make_unique(Args&& ... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
  }
}

#endif

// Local variables:
// mode: c++
// End:
