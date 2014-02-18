#define BOOST_TEST_MODULE ( make_shared test )
#include "boost/test/auto_unit_test.hpp"

#include "cetlib/make_unique.h"
#include <typeinfo>

BOOST_AUTO_TEST_SUITE ( MakeSharedTests )

BOOST_AUTO_TEST_CASE ( simple )
{
  int const val = 3;
  auto p = cet::make_unique<int>(val);
  BOOST_CHECK(p != nullptr);
  BOOST_REQUIRE_EQUAL(*p, val);
  BOOST_CHECK(typeid(p) == typeid(std::unique_ptr<int>));
}

BOOST_AUTO_TEST_SUITE_END()

