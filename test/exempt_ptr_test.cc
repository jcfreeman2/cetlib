#include "cetlib/exempt_ptr.h"


void
  ensure( int which, bool claim )
{
  if( not claim )
    std::exit(which);
}


int
  main( )
{

  return 0;

}  // main()
