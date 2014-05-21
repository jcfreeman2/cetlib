#ifndef cetlib_PluginFactory_h
#define cetlib_PluginFactory_h
#include "cetlib/LibraryManager.h"
#include "cetlib/PluginDefs.h"
#include "cetlib/detail/wrapLibraryManagerException.h"
#include "cetlib/exception.h"

#include <memory>
#include <string>

namespace cet {
  template <typename PLUGIN_TYPE,
            typename RESULT_TYPE,
            typename... ARGS>
  class PluginFactory;
}

template <typename PLUGIN_TYPE,
          typename RESULT_TYPE,
          typename... ARGS>
class cet::PluginFactory {
public:
  explicit
  PluginFactory(std::string const & suffix = "plugin",
                std::string const & makerName = "make_plugin",
                std::string const & pluginTypeFuncName = "pluginType");

  PluginFactory(PluginFactory const &) = delete;
  PluginFactory & operator = (PluginFactory const &) = delete;

  void setDiagReleaseVersion(std::string rv);
  void setDiagReleaseVersion(std::function<std::string ()> rvf);

  std::string pluginType(std::string const & libspec);

  RESULT_TYPE makePlugin(std::string const & libspec,
                         ARGS... args);

private:
  std::string releaseVersion_();

  LibraryManager lm_;
  std::string const makerName_;
  std::string const pluginTypeFuncName_;
  std::string releaseVersionString_;
  std::function<std::string ()> releaseVersionFunc_;
};

template <typename PLUGIN_TYPE,
          typename RESULT_TYPE,
          typename... ARGS>
cet::PluginFactory<PLUGIN_TYPE, RESULT_TYPE, ARGS...>::
PluginFactory(std::string const & suffix,
              std::string const & makerName,
              std::string const & pluginTypeFuncName)
  :
  lm_(suffix),
  makerName_(makerName),
  pluginTypeFuncName_(pluginTypeFuncName),
  releaseVersionString_(),
  releaseVersionFunc_()
{
}

template <typename PLUGIN_TYPE,
          typename RESULT_TYPE,
          typename... ARGS>
inline
void
cet::PluginFactory<PLUGIN_TYPE, RESULT_TYPE, ARGS...>::
setDiagReleaseVersion(std::function<std::string ()> rvf)
{
  releaseVersionFunc_ = rvf;
}

template <typename PLUGIN_TYPE,
          typename RESULT_TYPE,
          typename... ARGS>
inline
void
cet::PluginFactory<PLUGIN_TYPE, RESULT_TYPE, ARGS...>::
setDiagReleaseVersion(std::string rv)
{
  releaseVersionString_ = std::move(rv);
}

template <typename PLUGIN_TYPE,
          typename RESULT_TYPE,
          typename... ARGS>
std::string
cet::PluginFactory<PLUGIN_TYPE, RESULT_TYPE, ARGS...>::
pluginType(std::string const & libspec)
{
  PluginTypeFunc_t symbol = nullptr;
  try {
    lm_.getSymbolByLibspec(libspec, pluginTypeFuncName_, symbol);
  }
  catch (exception & e) {
    detail::wrapLibraryManagerException(e,
                                        "Plugin",
                                        libspec,
                                        releaseVersion_());
  }
  if (symbol == nullptr) {
    throw exception("Configuration", "BadPluginLibrary")
        << "Plugin " << libspec
        << " with version " << releaseVersion_()
        << " has internal symbol definition problems: consult an expert.";
  }
  return (*symbol)();
}

template <typename PLUGIN_TYPE,
          typename RESULT_TYPE,
          typename... ARGS>
RESULT_TYPE
cet::PluginFactory<PLUGIN_TYPE, RESULT_TYPE, ARGS...>::
makePlugin(std::string const & libspec,
           ARGS... args)
{
  PluginMaker_t<PLUGIN_TYPE, RESULT_TYPE, ARGS...> symbol = nullptr;
  try {
    lm_.getSymbolByLibspec(libspec, makerName_, symbol);
  }
  catch (exception & e) {
    detail::wrapLibraryManagerException(e,
                                        "Plugin",
                                        libspec,
                                        releaseVersion_());
  }
  if (symbol == nullptr) {
    throw exception("Configuration", "BadPluginLibrary")
      << "Plugin " << libspec
      << " with version " << releaseVersion_()
      << " has internal symbol definition problems: consult an expert.";
  }
  return (*symbol)(args...);
}

template <typename PLUGIN_TYPE,
          typename RESULT_TYPE,
          typename... ARGS>
std::string
cet::PluginFactory<PLUGIN_TYPE, RESULT_TYPE, ARGS...>::
releaseVersion_()
{
  std::string result;
  if (releaseVersionFunc_) {
    result = releaseVersionFunc_();
  } else {
    result = releaseVersionString_.empty() ?
             "Unknown" :
             releaseVersionString_;
  }
  return result;
}
#endif /* cetlib_PluginFactory_h */

// Local Variables:
// mode: c++
// End:
