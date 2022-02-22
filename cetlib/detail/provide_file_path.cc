#include "cetlib/detail/provide_file_path.h"
#include "boost/filesystem.hpp"

std::string
cet::detail::provide_file_path(std::string const& filename)
{
  namespace bfs = boost::filesystem;
  bfs::path const p{filename};
  return bfs::complete(p).native();
}
