#include "cetlib/BasicPluginFactory.h"

cet::BasicPluginFactory::
BasicPluginFactory(std::string  const & suffix,
                   std::string const & makerName,
                   std::string const & pluginTypeFuncName)
:
  PluginFactory(suffix),
  makerName_(makerName),
  pluginTypeFuncName_(pluginTypeFuncName)
{
}
