#include "cetlib/exception.h"
#include <cstdlib>
#include <iostream>


using cet::exception;


void
  ensure( int which, bool claim )
{
  if( not claim )
    std::exit(which);
}


int
  main( )
{
  exception e("Hello.");

  e << "I greet you." << 16 << std::endl;
  std::cout << e;

  return 0;

}  // main()
