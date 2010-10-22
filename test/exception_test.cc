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


struct my_exception
  : public cet::exception
{

  my_exception( Category const & category )
    : cet::exception( category )
  { }

};  // my_exception


int
  main( )
{
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
      ensure( 3, e.category() == "Bye" );
      std::string s = e.explain_self();
      ensure( 4, s.find("Nice meeting you") != std::string::npos );
    }
  }

  {
    ensure( 11, my_exception("Test").category() == "Test" );

    my_exception e("Hello");
    ensure( 12, e.category() == "Hello" );

    try {
      throw my_exception("Bye") << "Nice meeting you" << std::endl;
    }
    catch( my_exception const & e ) {
      std::string s = e.explain_self();
      ensure( 13, s.find("Nice meeting you") != std::string::npos );
    }
    catch( exception const & e ) {
      ensure( 14, false );
    }
  }

  return 0;

}  // main()
