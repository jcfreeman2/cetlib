// ======================================================================
//
// filesystem - file-level utilities
//
// ======================================================================

#include "cetlib/filesystem.h"

#include <sys/stat.h>  // stat

// ----------------------------------------------------------------------

bool
  cet::file_exists( std::string const & qualified_filename )
{
  struct stat file_info;
  return 0 == stat( qualified_filename.c_str()
                  , & file_info
                  );
}

// ======================================================================
