// ======================================================================
//
// demangle: Call the cross-vendor API to demangle symbols
//           (eg for ROOT dictionary errors).
//
// ======================================================================

#include "cetlib/demangle.h"

#include "cetlib/split.h"
#include <cstdlib>
#include <cxxabi.h>
#include <sstream>
#include <vector>

namespace {

  struct buffer_sentry
  {
    buffer_sentry() : buf(nullptr) { }
    ~buffer_sentry() noexcept { free(buf); }

    char * buf;
  };  // buffer_sentry

}

std::string cet::demangle_symbol(std::string const &mangled) {
  buffer_sentry unmangled; // Auto-free of buffer on exit from function
  size_t length;
  int status;
  unmangled.buf = abi::__cxa_demangle(mangled.c_str(),
                                      unmangled.buf,
                                      &length,
                                      &status);
  if (unmangled.buf == nullptr) return mangled; // Failure
  else                          return unmangled.buf;
}

std::string cet::demangle_message(std::string const &message) {
  std::ostringstream result;
  std::vector<std::string> words;
  cet::split(message, ' ', std::back_inserter(words));
  for (std::vector<std::string>::const_iterator
         b = words.begin(),
         i = b,
         e = words.end();
       i != e;
       ++i) {
    if (i != b) result << " ";
    result << cet::demangle_symbol(*i);
  }
  return result.str();
}

std::string cet::demangle(std::string const &mangled) {
  return demangle_symbol(mangled);
}
