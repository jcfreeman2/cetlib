#ifndef CETLIB_INCLUDE_H
#define CETLIB_INCLUDE_H

// ======================================================================
//
// include - file ->string, interpolating #include'd files along the way
//
// ======================================================================

#include <fstream>
#include <string>

namespace cet {

  void
    include( std::istream & in
           , std::string  & result
           );

  void
    include( std::istream      & in
           , std::string const & search_path_arg
           , std::string       & result
           );

}

// ======================================================================

#endif
