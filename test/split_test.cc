#include "cetlib/split.h"
#include <cstdlib>
#include <iterator>
#include <string>
#include <vector>
#include <cassert>


using cet::split;


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
    std::string s;
    std::vector<std::string> v;
    split( s, ':', std::back_inserter(v) );
    ensure( 1, v.size() == 0 );
  }

  {
    std::string s("abc:d:ef:");
    std::vector<std::string> v;
    split( s, ':', std::back_inserter(v) );
    ensure( 11, v.size() == 3 );
    ensure( 12, v[0] == "abc" );
    ensure( 13, v[1] == "d" );
    ensure( 14, v[2] == "ef" );
  }
  return 0;
}
