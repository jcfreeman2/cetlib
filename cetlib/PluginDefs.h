#ifndef cetlib__PluginDefs_h
#define cetlib__PluginDefs_h

#include <memory>
#include <string>

// Function pointer typedefs (for internal use by art).
namespace cet {
  template <typename PLUGIN_TYPE,
            typename RESULT_TYPE,
            typename... ARGS>
  using PluginMaker_t = RESULT_TYPE (*) (ARGS...);
  using PluginTypeFunc_t = std::string (*)();
}
#endif /* cetlib__PluginDefs_h */

// Local Variables:
// mode: c++
// End:
