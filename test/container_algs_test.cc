#define BOOST_TEST_MODULE ( container_algorithms test )
#include "boost/test/auto_unit_test.hpp"
#include "boost/test/test_tools.hpp"
#include "cetlib/container_algorithms.h"
#include <map>
#include <utility>

namespace {

  using cet::cbegin;
  using cet::cend;

  template <class T>
  struct A {
    A(T t) : t_(t) {}
    T t_;

    bool operator<(A<T> const& r) const
    {
      return t_ < r.t_;
    }

    bool operator==(A<T> const& r) const
    {
      return t_ == r.t_;
    }

    bool operator!=(A<T> const& r) const
    {
      return !operator==(r);
    }
  };

  template<class T>
  std::ostream& operator<<(std::ostream& os, A<T> const& a)
  {
    return os << a.t_;
  }

  template <class T>
  struct MakeA {
    auto operator()(T const val) const { return A<T>{val}; }
  };

  template <class T, class U>
  struct MakeAPair {
    auto operator()(T const t, U const u) const { return std::pair<T,U>{t,u}; }
  };

}

BOOST_AUTO_TEST_SUITE( container_algorithms )

BOOST_AUTO_TEST_CASE( transform_all ) {

  std::vector<int>  const v1 { 1, 2, 3, 4 };
  std::vector<char> const v2 { 'a', 'b', 'c', 'd' };

  // One-input version
  std::vector<A<int>> is1, is2;
  std::vector<A<char>> cs1, cs2;
  std::map<A<int>,A<char>> p1, p2;

  std::transform( cbegin(v1), cend(v1), std::back_inserter(is1), MakeA<int>() );
  std::transform( cbegin(v2), cend(v2), std::back_inserter(cs1), MakeA<char>() );
  std::transform( cbegin(v1), cend(v1), cbegin(v2), std::inserter(p1, begin(p1)), MakeAPair<int,char>() );

  cet::transform_all( v1, std::back_inserter(is2), MakeA<int>() );
  cet::transform_all( v2, std::back_inserter(cs2), MakeA<char>() );
  cet::transform_all( v1, v2, std::inserter(p2, begin(p2)), MakeAPair<int,char>() );

  BOOST_CHECK_EQUAL_COLLECTIONS( cbegin(is1), cend(is1), cbegin(is2), cend(is2) );
  BOOST_CHECK_EQUAL_COLLECTIONS( cbegin(cs1), cend(cs1), cbegin(cs2), cend(cs2) );

  BOOST_CHECK_EQUAL( p1.size(), p2.size() );

  auto p1_it = cbegin(p1);
  auto p2_it = cbegin(p1);
  for ( ; p1_it != cend(p1) ; ++p1_it, ++p2_it ) {
    BOOST_CHECK_EQUAL( p1_it->first.t_ , p2_it->first.t_ );
    BOOST_CHECK_EQUAL( p1_it->second.t_, p2_it->second.t_ );
  }

}
BOOST_AUTO_TEST_SUITE_END()
