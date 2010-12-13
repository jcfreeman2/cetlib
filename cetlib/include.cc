// ======================================================================
//
// include - file ->string, interpolating #include'd files along the way
//
// ======================================================================

#include "cetlib/include.h"

// ----------------------------------------------------------------------

bool
  cet::include( std::istream & in
              , std::string  & result
              )
{
  static std::string const include_lit("#include \"");
  std::size_t include_sz = include_lit.size();

  for( std::string line; std::getline(in, line);  ) {
    if( line.find(include_lit) == 0 ) {
      if( line.end()[-1] != '\"' )
        return false;
      std::ifstream f( line.substr( include_sz
                                  , line.size() - include_sz - 1
                                  ).c_str()
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
