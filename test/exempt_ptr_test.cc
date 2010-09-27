#include "cetlib/exempt_ptr.h"
#include <cstdlib>


using cet::exempt_ptr;


void
  ensure( int which, bool claim )
{
  if( not claim )
    std::exit(which);
}


int
  main( )
{
  exempt_ptr<int> p( new int(16) );
  ensure( 1, *p == 16 );

  exempt_ptr<int> q( p );
  ensure( 2, *p == *q );
  ensure( 3, p == q );

  p.reset( new int(0) );
  return *p;

}  // main()
