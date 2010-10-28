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

  {
    std::string s(":");
    std::vector<std::string> v;
    split(s, ':', std::back_inserter(v));
    ensure( 21, v.size() == 2 );
    ensure( 22, v[0].empty() );
    ensure( 23, v[1].empty() );
  }

  {
    std::string s(":a:");
    std::vector<std::string> v;
    split(s, ':', std::back_inserter(v));
    ensure( 31, v.size() ==3 );
    ensure( 32, v[0].empty() );
    ensure( 33, v[1] == "a" );
    ensure( 34, v[2].empty() );
  }

  {
    std::string s(":ab");
    std::vector<std::string> v;
    split(s, ':', std::back_inserter(v));
    ensure( 41, v.size() == 2 );
    ensure( 42, v[0].empty() );
    ensure( 43, v[1] == "ab" );
  }

  return 0;
}
