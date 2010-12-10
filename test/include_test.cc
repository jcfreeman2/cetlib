#include "cetlib/include.h"
#include <cstdlib>
#include <fstream>

using cet::include;


void
  ensure( int which, bool claim )
{
  if( not claim )
    std::exit(which);
}


int
  main( )
{
  ensure( 1, true );

  return 0;

}  // main()
