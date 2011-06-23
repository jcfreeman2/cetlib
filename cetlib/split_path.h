#ifndef CETLIB_SPLIT_PATH_H
#define CETLIB_SPLIT_PATH_H

// ======================================================================
//
// split_path: Split the string 'path' into components delimited by a
//             single colon. Adjacent colons result in an empty string.
//
// ======================================================================

#include <string>
#include <vector>

namespace cet
{
  void
    split_path( std::string const        & path
              , std::vector<std::string> & components
              );
}

// ======================================================================

#endif
