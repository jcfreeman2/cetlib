#ifndef CETLIB_FILE_EXISTS_H
#define CETLIB_FILE_EXISTS_H

// ======================================================================
//
// filesystem - file-level utilities
//
// ======================================================================

#include <string>

namespace cet {

  bool
    file_exists( std::string const & qualified_filename );

}

// ======================================================================

#endif
