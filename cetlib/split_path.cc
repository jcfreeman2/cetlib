#include "cetlib/split_path.h"
#include "boost/algorithm/string.hpp"

void cet::split_path(std::string const& path, std::vector<std::string>& components)
{
  boost::algorithm::split(components, path, boost::algorithm::is_any_of(":"));
}
