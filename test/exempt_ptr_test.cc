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
  {
    exempt_ptr<int> p;
    ensure( 11, !p );
    ensure( 12, p == nullptr );
    ensure( 13, nullptr == p );
  }

  {
    exempt_ptr<int> p( new int(16) );
    ensure( 21, p != nullptr );
    ensure( 22, nullptr != p );
    ensure( 23, *p == 16 );

    exempt_ptr<int> q( p );
    ensure( 24, p == q );
    ensure( 25, *p == *q );

    p.reset( new int(0) );
    return *p;
  }

}  // main()
