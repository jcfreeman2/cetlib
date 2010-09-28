#include "cetlib/exception.h"
#include "cetlib/search_path.h"
#include <cstdlib>
#include <string>


using cet::exception;
using cet::search_path;


void
  ensure( int which, bool claim )
{
  if( not claim )
    std::exit(which);
}


int
  main( )
{
  {
    search_path sp( "$HOME:${HOME}:$(HOME)" );
    ensure( 1, sp.size() == 3 );
    std::string const & s = sp[0];
    ensure( 2, s[0] == '/' );
    ensure( 3, sp[1] == s );
    ensure( 4, sp[2] == s );
  }

  {
    try {
      search_path sp( "$xyzzy" );
      ensure( 11, false );
    }
    catch( exception const & e ) {
      ensure( 12, e.category() == "SEARCH_PATH" );
    }
  }

  {
    search_path sp( "" );
    ensure( 21, sp.empty() );
  }

  {
    search_path sp( "/tmp" );
    std::string const & s = sp.find_file("");
    ensure( 31, s == "/tmp/" );
  }

  return 0;
}
