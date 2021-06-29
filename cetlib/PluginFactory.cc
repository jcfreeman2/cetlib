#include "cetlib/PluginFactory.h"

#include <string>

cet::PluginFactory::PluginFactory(cet::search_path const& search_path,
                                  std::string const& suffix)
  : lm_{search_path, suffix}
{}

std::string
cet::PluginFactory::releaseVersion_() const
{
  std::string result;
  if (releaseVersionFunc_) {
    result = releaseVersionFunc_();
  } else {
    result = releaseVersionString_.empty() ? "Unknown" : releaseVersionString_;
  }
  return result;
}
