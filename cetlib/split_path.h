#ifndef CETLIB_SPLIT_PATH_h
#define CETLIB_SPLIT_PATH_h

#include <string>
#include <vector>

namespace cet
{
  // Split the string 'path' into components delimited by a single
  // colon. Adjacent colons result in an empty string.
  void split_path(std::string const& path, std::vector<std::string>& components);
}

#endif //  CETLIB_SPLIT_PATH_h

// Local Variables:
// mode: c++
// End:
