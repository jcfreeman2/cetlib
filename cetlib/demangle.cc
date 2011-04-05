#include "cetlib/demangle.h"

#include <cxxabi.h>
#include <cstdlib>
#include "cetlib/zero_init.h"

namespace {
  struct buffer_sentry {
    buffer_sentry() : buf(0) { }
    ~buffer_sentry() { free(buf); }
    char * buf;
  };
}

std::string cet::demangle(std::string const &mangled) {
  buffer_sentry unmangled; // Auto-free of buffer on exit from function
  size_t length;
  int status;
  unmangled.buf = abi::__cxa_demangle(mangled.c_str(),
                                      unmangled.buf,
                                      &length,
                                      &status);
  if (unmangled.buf == NULL) {
    return mangled; // Failure
  } else {
    std::string result(unmangled.buf);
    return result;
  }
}
