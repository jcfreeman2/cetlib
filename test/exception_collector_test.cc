#include "cetlib/exception_collector.h"
#include <iostream>


using cet::exception_collector;


void
  ensure( int which, bool claim )
{
  if( not claim )
    std::exit(which);
}


int calls_left = 2;
void  f()  { --calls_left; throw 1; }
void  g()  { --calls_left; throw 2; }


int
  main( )
{
  exception_collector ec;

  ensure( 1, calls_left == 2 );
  ec.call( f );
  ensure( 2, calls_left == 1 );
  ec.call( g );
  ensure( 3, calls_left == 0 );

  return 0;

}  // main()
