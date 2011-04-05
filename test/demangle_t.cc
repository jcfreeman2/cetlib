#define BOOST_TEST_MODULE ( demangle test )
#include "boost/test/auto_unit_test.hpp"

#include "cetlib/demangle.h"

#include <iostream>

BOOST_AUTO_TEST_SUITE ( DemangleTests )

BOOST_AUTO_TEST_CASE ( type )
{
  BOOST_REQUIRE(cet::demangle("t") == "unsigned short");
}

BOOST_AUTO_TEST_CASE ( vtable )
{
  BOOST_REQUIRE(cet::demangle("_ZTVN3cet9exceptionE") ==
                "vtable for cet::exception");
}

BOOST_AUTO_TEST_CASE ( function )
{
  BOOST_REQUIRE(cet::demangle("_ZN3cet8demangleERKSs") ==
                "cet::demangle(std::string const&)");
}

BOOST_AUTO_TEST_CASE ( fail )
{
  BOOST_REQUIRE(cet::demangle("_ZN13Xrawfileparser13RawFileParserD1Ev") ==
                "_ZN13Xrawfileparser13RawFileParserD1Ev");
}

BOOST_AUTO_TEST_SUITE_END()
