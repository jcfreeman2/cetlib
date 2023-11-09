#define BOOST_TEST_MODULE (exempt_ptr test)

#include "boost/test/unit_test.hpp"
#include "cetlib/exempt_ptr.h"
#include <cstdlib>

namespace cet {
  template <typename T>
  std::ostream&
  boost_test_print_type(std::ostream& os, exempt_ptr<T> p)
  {
    return os << p.get();
  }
}

using cet::exempt_ptr;
using cet::make_exempt_ptr;

BOOST_AUTO_TEST_SUITE(exempt_ptr_test)

BOOST_AUTO_TEST_CASE(null_ptr_test)
{
  exempt_ptr<int> p;
  BOOST_TEST(!p);
  BOOST_TEST(p == nullptr);
  BOOST_TEST(nullptr == p);
  BOOST_TEST((p == 0));
  BOOST_TEST((0 == p));
}

BOOST_AUTO_TEST_CASE(constructibility)
{ // non-const => const
  static_assert(std::is_constructible_v<exempt_ptr<int const>, int*>);
  // const => non-const (not constructible)
  static_assert(not std::is_constructible_v<exempt_ptr<int>, int const*>);
  // cet::exempt_ptr<T> ==> cet::exempt_ptr<T const>
  static_assert(
    std::is_constructible_v<exempt_ptr<int const>, exempt_ptr<int>>);
}

BOOST_AUTO_TEST_CASE(deduction_guide)
{
  int i{42};
  exempt_ptr p{&i};
  static_assert(std::is_same_v<exempt_ptr<int>, decltype(p)>);
  int const ci{42};
  exempt_ptr cp{&ci};
  static_assert(std::is_same_v<exempt_ptr<int const>, decltype(cp)>);
}

BOOST_AUTO_TEST_CASE(comparisons)
{
  int i{42};
  exempt_ptr p{&i};
  exempt_ptr<int const> cp{&i};
  BOOST_TEST(p == cp);
  BOOST_TEST(cp == p);
}

BOOST_AUTO_TEST_CASE(nondeletable_ptr)
{
  double const d{42.0};
  auto p = make_exempt_ptr(&d);
  static_assert(std::is_same_v<exempt_ptr<double const>, decltype(p)>);
  BOOST_TEST(p != nullptr);
  BOOST_TEST(nullptr != p);
  BOOST_TEST(*p == 42.0);
}

BOOST_AUTO_TEST_CASE(ptr_with_value)
{
  int i{16};
  exempt_ptr<int> p(&i);
  BOOST_TEST(p != nullptr);
  BOOST_TEST(nullptr != p);
  BOOST_TEST(*p == 16);

  exempt_ptr<int> q(p);
  BOOST_TEST(p == q);
  BOOST_TEST(*p == *q);

  int j{};
  p.reset(&j);
  BOOST_TEST(*p == 0);
}

BOOST_AUTO_TEST_SUITE_END()
