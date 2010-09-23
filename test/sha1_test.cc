#include "cetlib/sha1.h"
  using cet::sha1;

#include <string>



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
    sha1 digest;
    digest.digest();
  }

  {
    sha1 d1("hi");
    sha1 d2; d2 << "hi";
    ensure( 1, d1.digest() == d2.digest() );
  }

  return 0;

}  // main()
