// ======================================================================
//
// include - file ->string, interpolating #include'd files along the way
//
// ======================================================================

#include "cetlib/include.h"

// ----------------------------------------------------------------------

bool
  cet::include( std::ifstream & in
              , std::string   & result
              )
{
  static std::string const include_lit("#include \"");

  for( std::string line; std::getline(in, line);  ) {
    if( line.find(include_lit) ) {
      if( line.end()[-1] != '\"' )
        return false;
      std::ifstream f( line.erase(0, include_lit.size())
                           .erase(line.size()-1, 1)
                           .c_str()
                     , std::ios_base::in
                     );
      if( ! f || ! include(f, result) )
        return false;
    }
    else {
      result.append(line)
            .append(1, '\n');
      continue;
    }
  }  // for

  return true;

}  // include()

// ======================================================================
