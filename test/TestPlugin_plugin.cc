#include "TestPluginBase.h"

#include "cetlib/make_unique.h"

#include <memory>

namespace cettest {
  class TestPlugin;
}

class cettest::TestPlugin : public TestPluginBase {
public:
  TestPlugin(std::string message);
};

cettest::TestPlugin::
TestPlugin(std::string message)
:
  TestPluginBase(std::move(message))
{
}

extern "C" {
  std::unique_ptr<cettest::TestPluginBase>
  make_plugin(std::string message)
  {
    return cet::make_unique<cettest::TestPlugin>(std::move(message));
  }
  std::string
  pluginType()
  {
    return cet::PluginTypeDeducer<cettest::TestPluginBase>::value;
  }
}
