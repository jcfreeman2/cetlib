// ======================================================================
//
// test cet::search_path
//
// ======================================================================

#include "cetlib/exception.h"
#include "cetlib/search_path.h"
#include <cstdlib>
#include <string>


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
    //ensure( 1, search_path("xyzzy").size() == 1 );
    ensure( 2, search_path(":xyzzy").size() == 1 );
    ensure( 3, search_path(":xyzzy:").size() == 1 );
    ensure( 4, search_path("xyzzy:plugh").size() == 2 );
    ensure( 5, search_path("xyzzy:::plugh").size() == 2 );
    ensure( 6, search_path("xyzzy:plugh:twisty:grue").size() == 4 );
  }

  {
    try {
      search_path(":");
      ensure( 11, false );
    }
    catch( cet::exception const & e ) {
      ensure( 12, e.category() == "search_path" );
    }
  }

  {
    try {
      search_path sp( ":/tmp:" );
      std::string const & s = sp.find_file("");
      ensure( 21, false );
    }
    catch( cet::exception const & e ) {
      ensure( 22, e.category() == "search_path" );
    }
  }

  return 0;
}
