#define BOOST_TEST_MODULE ( simple_stats test )
#include "boost/test/auto_unit_test.hpp"

#include "cetlib/simple_stats.h"
#include "cpp0x/cmath"

using cet::simple_stats;

BOOST_AUTO_TEST_SUITE( simple_stats_test )

BOOST_AUTO_TEST_CASE( default_ctor_test )
{
  simple_stats  s;
  BOOST_CHECK_EQUAL( s.size(), 0u );
  BOOST_CHECK( std::isinf(s.max()) );
  BOOST_CHECK( std::isinf(s.min()) );
  BOOST_CHECK( std::isinf(s.small()) );
  BOOST_CHECK_EQUAL( s.sum(), 0.0 );
  BOOST_CHECK_EQUAL( s.sumsq(), 0.0 );
}

BOOST_AUTO_TEST_CASE( value_ctor_test )
{
  double pi = 3.14;
  simple_stats  s(pi);
  BOOST_CHECK_EQUAL( s.size(), 1u );
  BOOST_CHECK_EQUAL( s.max(), pi );
  BOOST_CHECK_EQUAL( s.min(), pi );
  BOOST_CHECK_EQUAL( s.small(), pi );
  BOOST_CHECK_EQUAL( s.sum(), pi );
  BOOST_CHECK_EQUAL( s.sumsq(), pi * pi );
}

BOOST_AUTO_TEST_CASE( use_test )
{
  double pi = 3.14;
  simple_stats  s(pi); s.use(-pi);
  BOOST_CHECK_EQUAL( s.size(), 2u );
  BOOST_CHECK_EQUAL( s.max(), pi );
  BOOST_CHECK_EQUAL( s.min(), -pi );
  BOOST_CHECK_EQUAL( s.small(), pi );
  BOOST_CHECK_EQUAL( s.sum(), 0.0 );
  BOOST_CHECK_EQUAL( s.sumsq(), 2 * pi * pi );
}

BOOST_AUTO_TEST_SUITE_END()
