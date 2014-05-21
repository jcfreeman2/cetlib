#ifndef cetlib_PluginTypeDeducer_h
#define cetlib_PluginTypeDeducer_h

////////////////////////////////////////////////////////////////////////
// PluginTypeDeducer
//
// Used by PluginFactory and users thereof to ascertain and verify the
// correct plugin type for a plugin.
//
// Specialize this class for your own plugin type and make sure it is
// seen before being required. A reasonable place would be the in the
// header file of the plugin type's base class, immediately below the
// forward declaration of the base class name.
////////////////////////////////////////////////////////////////////////

#include <string>

namespace cet {
  // PluginTypeDeducer.
  template <typename T>
    struct PluginTypeDeducer;
}

template <typename T>
struct cet::PluginTypeDeducer {
  static std::string const value;
};

template <typename T>
std::string const
cet::PluginTypeDeducer<T>::
value = "Unknown";

#endif /* cetlib_PluginTypeDeducer_h */

// Local Variables:
// mode: c++
// End:
