// ======================================================================
//
// test registry_via_id
//
// ======================================================================

#include "cetlib/registry_via_id.h"
#include <cstdlib>

using cet::registry_via_id;


void
  ensure(  int which, bool claim )
{
  if( not claim )
    std::exit(which);
}


struct Int
{
  int i;
  Int(int i = 0) : i(i) { }
  float id() const { return i; }
  bool operator == ( int other ) const { return i == other; }
};


int
  main( )
{
  Int i;

  typedef  registry_via_id<float, Int>  reg;
  ensure( 1, reg::empty() );
  ensure( 2, reg::size() == 0 );

  reg::put(1);
  reg::put(2);
  reg::put(3);
  ensure( 3, reg::size() == 3 );
  reg::put(4);
  reg::put(5);
  reg::put(6);
  reg::put(7);
  reg::put(8);
  reg::put(9);
  reg::put(0);

  ensure( 11, ! reg::empty() );
  ensure( 12, reg::size() == 10 );
  ensure( 13, reg::get(7.0F) == 7 );
  ensure( 14, reg::get(5.0F) == 5 );
  ensure( 15, ! reg::get(3.14F, i) );
  ensure( 16, reg::get(2.0F, i) && i == 2 );

  try {
    reg::get(10.0F);
    ensure( 21, false );
  }
  catch( cet::exception const & e ) {
    ensure( 22, e.category() == "cet::registry_via_id" );
    std::string s = e.explain_self();
    ensure( 23, s.find("not found in registry") != std::string::npos );
  }
  catch( ... ) {
    ensure( 24, false );
  }

  return 0;

}  // main()
