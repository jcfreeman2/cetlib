#ifndef CETLIB_FILESYSTEM_H
#define CETLIB_FILESYSTEM_H

// ======================================================================
//
// filesystem: A family of file-level utilities
//
// ======================================================================

#include <string>

namespace cet {

  bool
    file_exists( std::string const & qualified_filename );

  bool
    is_absolute_filepath( std::string const & qualified_filename );

  bool
    is_relative_filepath( std::string const & qualified_filename );

}

// ======================================================================

#endif
