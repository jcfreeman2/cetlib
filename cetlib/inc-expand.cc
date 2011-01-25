// ======================================================================
//
// inc-expand: read/write specified text, replacing each #include
//             directive with the corresponding file's contents
//
// ======================================================================

#include "cetlib/include.h"
#include <fstream>
#include <iostream>
#include <string>

// ----------------------------------------------------------------------

bool
  is_single_dash( char const * s ) { return s[0] == '-' && s[1] == '\0'; }

// ----------------------------------------------------------------------

int
  do_including( std::istream & from, std::ostream & to )
{
  std::string result;
  if( ! cet::include(from, result) )
    return 1;

  to << result;
  return 0;

}  // do_including()

// ----------------------------------------------------------------------

int
  main( int argc, char* argv[] )
{
  int nfailures = 0;

  if( argc == 1 )
    nfailures += do_including(std::cin, std::cout);
  else {
    for( int k = 1; k != argc; ++k ) {
      if( is_single_dash(argv[k]) )
        nfailures += do_including(std::cin, std::cout);
      else {
        std::ifstream from(argv[k], std::ios_base::in);
        nfailures += do_including(from, std::cout);
      }
    }
  }

  return nfailures;

}  // main()
