#include "cetlib/value_ptr.h"
#include <cstdlib>


using cet::value_ptr;


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
    value_ptr<int> p;
    ensure( 1, !p );
    ensure( 2, p == nullptr );
    ensure( 3, nullptr == p );
    ensure( 4, p == 0 );
    ensure( 5, 0 == p );
  }

  {
    value_ptr<int> p( new int(16) );
    ensure( 11, bool(p) );
    ensure( 12, *p == 16 );

    value_ptr<int> q( p );
    ensure( 13, *p == *q );
    ensure( 14, p != q );

    p.reset( new int(0) );
    return *p;
  }

}  // main()
