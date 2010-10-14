#include "cetlib/coded_exception.h"
#include <cstdlib>


using cet::coded_exception;


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
