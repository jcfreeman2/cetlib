#define BOOST_TEST_MODULE ( name_of test )
#include "boost/test/auto_unit_test.hpp"

#include "cetlib/name_of.h"

using cet::name_of;

struct sometype;

BOOST_AUTO_TEST_SUITE( name_of_test )

BOOST_AUTO_TEST_CASE( unknown_t_test )
{
  BOOST_CHECK_EQUAL( name_of<sometype   >::is(), "unknown-type"             );
  BOOST_CHECK_EQUAL( name_of<sometype * >::is(), "ptr-to_unknown-type"      );
  BOOST_CHECK_EQUAL( name_of<sometype & >::is(), "ref-to_unknown-type"      );
  BOOST_CHECK_EQUAL( name_of<sometype[1]>::is(), "array[1]-of_unknown-type" );
  BOOST_CHECK_EQUAL( name_of<sometype[0]>::is(), "array[0]-of_unknown-type" );
}

BOOST_AUTO_TEST_CASE( fundamental_types_test )
{
  // signed ints:
  BOOST_CHECK_EQUAL( name_of<int        >::is(), "int"   );
  BOOST_CHECK_EQUAL( name_of<long       >::is(), "long"  );
  BOOST_CHECK_EQUAL( name_of<long long  >::is(), "llong" );
  BOOST_CHECK_EQUAL( name_of<short      >::is(), "short" );
  BOOST_CHECK_EQUAL( name_of<signed char>::is(), "schar" );

  // unsigned ints:
  BOOST_CHECK_EQUAL( name_of<unsigned char     >::is(), "uchar" );
  BOOST_CHECK_EQUAL( name_of<unsigned int      >::is(), "uint"   );
  BOOST_CHECK_EQUAL( name_of<unsigned long     >::is(), "ulong"  );
  BOOST_CHECK_EQUAL( name_of<unsigned long long>::is(), "ullong" );
  BOOST_CHECK_EQUAL( name_of<unsigned short    >::is(), "ushort" );

  // floating:
  BOOST_CHECK_EQUAL( name_of<double     >::is(), "double"  );
  BOOST_CHECK_EQUAL( name_of<float      >::is(), "float"   );
  BOOST_CHECK_EQUAL( name_of<long double>::is(), "ldouble" );

  // other:
  BOOST_CHECK_EQUAL( name_of<bool>::is(), "bool" );
  BOOST_CHECK_EQUAL( name_of<char>::is(), "char" );
  BOOST_CHECK_EQUAL( name_of<void>::is(), "void" );
}

BOOST_AUTO_TEST_CASE( composite_types_test )
{
  // const:
  BOOST_CHECK_EQUAL( name_of<int         const>::is(), "const_int"   );
  BOOST_CHECK_EQUAL( name_of<long        const>::is(), "const_long"  );
  BOOST_CHECK_EQUAL( name_of<long long   const>::is(), "const_llong" );
  BOOST_CHECK_EQUAL( name_of<short       const>::is(), "const_short" );
  BOOST_CHECK_EQUAL( name_of<signed char const>::is(), "const_schar" );

  // volatile:
  BOOST_CHECK_EQUAL( name_of<unsigned char      volatile>::is(), "volatile_uchar" );
  BOOST_CHECK_EQUAL( name_of<unsigned int       volatile>::is(), "volatile_uint"   );
  BOOST_CHECK_EQUAL( name_of<unsigned long      volatile>::is(), "volatile_ulong"  );
  BOOST_CHECK_EQUAL( name_of<unsigned long long volatile>::is(), "volatile_ullong" );
  BOOST_CHECK_EQUAL( name_of<unsigned short     volatile>::is(), "volatile_ushort" );

  // const-volatile:
  BOOST_CHECK_EQUAL( name_of<double      const volatile>::is(), "c-v_double"  );
  BOOST_CHECK_EQUAL( name_of<float       const volatile>::is(), "c-v_float"   );
  BOOST_CHECK_EQUAL( name_of<long double const volatile>::is(), "c-v_ldouble" );
  BOOST_CHECK_EQUAL( name_of<bool        const volatile>::is(), "c-v_bool" );
  BOOST_CHECK_EQUAL( name_of<char        const volatile>::is(), "c-v_char" );
  BOOST_CHECK_EQUAL( name_of<void        const volatile>::is(), "c-v_void" );
}

BOOST_AUTO_TEST_SUITE_END()
