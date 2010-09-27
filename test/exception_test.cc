#include "cetlib/exception.h"
#include <cstdlib>
#include <iostream>
#include <string>


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
  ensure( 1, exception("Test").category() == "Test" );

  exception e("Hello");
  ensure( 2, e.category() == "Hello" );

  e << "I greet you." << 16 << std::endl;
  std::cout << e;

  try {
    throw exception("Bye") << "Nice meeting you" << std::endl;
  }
  catch( exception const & e ) {
    std::string s = e.explain_self();
    ensure( 3, s.find("Nice meeting you") != std::string::npos );
  }

  return 0;

}  // main()
