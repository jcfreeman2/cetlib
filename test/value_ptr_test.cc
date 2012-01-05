
// ======================================================================
//
// test value_ptr
//
// ======================================================================

#define BOOST_TEST_MODULE ( value_ptr test )
#include "boost/test/auto_unit_test.hpp"

#include "cetlib/value_ptr.h"

using cet::value_ptr;

struct Base { virtual ~Base() { } };
struct Derived : public Base { };

BOOST_AUTO_TEST_SUITE( value_ptr_test )

BOOST_AUTO_TEST_CASE( nullptr_test )
{
  value_ptr<int> p;
  BOOST_CHECK( !p );
  BOOST_CHECK( p == nullptr );
  BOOST_CHECK( nullptr == p );
  BOOST_CHECK( p == 0 );
  BOOST_CHECK( 0 == p );
}

BOOST_AUTO_TEST_CASE( basic_test )
{
  value_ptr<int> p( new int(16) );
  BOOST_CHECK( bool(p) );
  BOOST_CHECK_EQUAL( *p, 16 );

  int * p_addr = p.get();
  value_ptr<int> q( p );
  BOOST_CHECK_EQUAL( *p, *q );
  BOOST_CHECK( p != q );
  BOOST_CHECK( p_addr != q.get() );

  p.reset( new int(0) );
  BOOST_CHECK_EQUAL( *p, 0 );
  BOOST_CHECK( p_addr != p.get() );
}

BOOST_AUTO_TEST_SUITE_END()
