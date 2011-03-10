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
  }

  {
    value_ptr<int> p( new int(16) );
    ensure( 11, p );
    ensure( 12, *p == 16 );

    value_ptr<int> q( p );
    ensure( 13, *p == *q );
    ensure( 14, p != q );

    p.reset( new int(0) );
    return *p;
  }

}  // main()
