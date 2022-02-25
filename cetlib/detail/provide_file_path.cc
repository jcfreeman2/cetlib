#include "cetlib/detail/provide_file_path.h"
#include "boost/filesystem.hpp"

std::string
cet::detail::provide_file_path(char const* filename)
{
  assert(filename);
  boost::filesystem::path const p{filename};
  return complete(p).native();
}
