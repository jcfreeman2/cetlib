#define BOOST_TEST_MODULE ( zero_init test )
#include "boost/test/auto_unit_test.hpp"

#include "cetlib/zero_init.h"

BOOST_AUTO_TEST_SUITE( zero_init )

BOOST_AUTO_TEST_CASE( default_behavior ) {
  { typedef  int  T;
    BOOST_CHECK( cet::zero_init<T>() == 0 );
    cet::zero_init<T> x;
    BOOST_CHECK( x == 0 );
    BOOST_CHECK( sizeof(x) == sizeof(T) );
    T & r = x;
    BOOST_CHECK( r == 0 );
  }
  { typedef  unsigned long  T;
    BOOST_CHECK( cet::zero_init<T>() == 0 );
    cet::zero_init<T> x;
    BOOST_CHECK( x == 0 );
    BOOST_CHECK( sizeof(x) == sizeof(T) );
    T & r = x;
    BOOST_CHECK( r == 0 );
  }
}

BOOST_AUTO_TEST_CASE( copy_behavior ) {
  { typedef  unsigned  T;
    cet::zero_init<T> x;
    cet::zero_init<T> y = x;
    BOOST_CHECK( x == y );
    BOOST_CHECK( y == x );
    BOOST_CHECK( (y = x) == x );
    BOOST_CHECK( (x = y) == y );
  }
  { typedef  long  T;
    cet::zero_init<T> x = 7L;
    BOOST_CHECK( x == 7L );
    cet::zero_init<T> y(x);
    BOOST_CHECK( y == 7L );
    BOOST_CHECK( y == x );
  }
}

BOOST_AUTO_TEST_CASE( const_behavior ) {
  { typedef  int const  T;
    cet::zero_init<T> x;
    T y = x;
    BOOST_CHECK( x == y );
    BOOST_CHECK( y == x );
    T & r = x;
    BOOST_CHECK( r == y );
  }
  { typedef  int  T;
    cet::zero_init<T> const x;
    T const y = x;
    BOOST_CHECK( x == y );
    BOOST_CHECK( y == x );
    T const & r = x;
    BOOST_CHECK( r == y );
  } }

BOOST_AUTO_TEST_CASE( conversions ) {
  {
    cet::zero_init<long> x = cet::zero_init<int >(16);
    cet::zero_init<int > y = cet::zero_init<long>(16L) + 0L;
    BOOST_CHECK( x == y );
    BOOST_CHECK( y == x + 0 );
  }
  {
    cet::zero_init<float> z;
    BOOST_CHECK( z == 0.0F );
  }
}

BOOST_AUTO_TEST_SUITE_END()
