#ifndef cetlib_detail_provide_file_path_h
#define cetlib_detail_provide_file_path_h

#include <string>

namespace cet::detail {
  // This function takes the value of __FILE__ and converts it to an
  // absolute path.
  std::string provide_file_path(char const* filename);
}

#endif /* cetlib_detail_provide_file_path_h */

// Local Variables:
// mode: c++
// End:
