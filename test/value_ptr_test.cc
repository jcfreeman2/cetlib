#include "cetlib/value_ptr.h"


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
  value_ptr<int> p( new int(16) );
  ensure( 1, *p == 16 );

  value_ptr<int> q( p );
  ensure( 2, *p == *q );
  ensure( 3, p != q );

  p.reset( new int(0) );
  return *p;

}  // main()
