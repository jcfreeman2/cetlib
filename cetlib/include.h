// ======================================================================
//
// include - file ->string, interpolating #include'd files along the way
//
// ======================================================================

#include <fstream>
#include <string>

namespace cet {

  bool
    include( std::istream & in
           , std::string  & result
           );

  bool
    include( std::istream      & in
           , std::string const & search_path_arg
           , std::string       & result
           );

}

// ======================================================================
