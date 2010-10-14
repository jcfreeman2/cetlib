#include "cetlib/coded_exception.h"
#include <cstdlib>
#include <map>
#include <string>


using cet::coded_exception;


void
  ensure( int which, bool claim )
{
  if( not claim )
    std::exit(which);
}


enum E { E1 = 1, E2, E3 };
typedef  std::map<E,std::string> Emap;

Emap const &
  mapper()
{
  static  Emap  m;
  if( m.empty() ) m[E1] = "E1"
                , m[E2] = "E2"
                , m[E3] = "E3"
                ;

  return m;
}


int
  main( )
{

  coded_exception<E,mapper> e(E1);

  return 0;

}  // main()
