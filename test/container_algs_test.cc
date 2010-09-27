#include "cetlib/container_algorithms.h"
#include <cstdlib>


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
