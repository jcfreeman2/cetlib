#define BOOST_TEST_MODULE ( map_vector test )

#include "boost/test/auto_unit_test.hpp"
#include "cetlib/map_vector.h"
#include <string>

using cet::map_vector;
using cet::map_vector_key;

BOOST_AUTO_TEST_SUITE( map_vector_test )

BOOST_AUTO_TEST_CASE( empty_test )
{
  map_vector_key k(3);
  {
    map_vector<int> m;
    BOOST_CHECK( m.empty() );
    BOOST_CHECK( m.size() == 0u );
    BOOST_CHECK( ! m.has(k) );
    BOOST_CHECK( m.begin() == m.end() );
    BOOST_CHECK( m.find(k) == m.end() );
    BOOST_CHECK( m.getOrNull(k) == 0 );
    BOOST_CHECK_THROW( m.getOrThrow(k), cet::exception );
  }

  {
    map_vector<int> const m;
    BOOST_CHECK( m.empty() );
    BOOST_CHECK( m.size() == 0u );
    BOOST_CHECK( ! m.has(k) );
    BOOST_CHECK( m.begin() == m.end() );
    BOOST_CHECK( m.find(k) == m.end() );
    BOOST_CHECK( m.getOrNull(k) == 0 );
    BOOST_CHECK_THROW( m.getOrThrow(k), cet::exception );
  }
}

BOOST_AUTO_TEST_CASE( nonempty_test )
{
  typedef  int  value_t;
  map_vector<value_t> m;
  std::size_t sz(0);

  {
    map_vector_key k(1);
    value_t  v(10 + k.asInt());
    m[k] = v;
    BOOST_CHECK( ! m.empty() );
    BOOST_CHECK( m.size() == ++sz );
    BOOST_CHECK( m.begin() != m.end() );
    BOOST_CHECK( m.has(k) );
    BOOST_CHECK( m.find(k)->second == v );
    BOOST_CHECK( *m.getOrNull(k) == v );
    BOOST_CHECK( m.getOrThrow(k) == v );
  }

  {
    map_vector_key k(3);
    value_t  v(10 + k.asInt());
    m[k] = v;
    BOOST_CHECK( ! m.empty() );
    BOOST_CHECK( m.size() == ++sz );
    BOOST_CHECK( m.begin() != m.end() );
    BOOST_CHECK( m.has(k) );
    BOOST_CHECK( m.find(k)->second == v );
    BOOST_CHECK( *m.getOrNull(k) == v );
    BOOST_CHECK( m.getOrThrow(k) == v );
  }

  {
    map_vector_key k(5);
    value_t  v(10 + k.asInt());
    m[k] = v;
    BOOST_CHECK( ! m.empty() );
    BOOST_CHECK( m.size() == ++sz );
    BOOST_CHECK( m.begin() != m.end() );
    BOOST_CHECK( m.has(k) );
    BOOST_CHECK( m.find(k)->second == v );
    BOOST_CHECK( *m.getOrNull(k) == v );
    BOOST_CHECK( m.getOrThrow(k) == v );
  }

  {
    map_vector_key k(2);
    value_t  v(10 + k.asInt());
    m[k] = v;
    BOOST_CHECK( ! m.empty() );
    BOOST_CHECK( m.size() == ++sz );
    BOOST_CHECK( m.begin() != m.end() );
    BOOST_CHECK( m.has(k) );
    BOOST_CHECK( m.find(k)->second == v );
    BOOST_CHECK( *m.getOrNull(k) == v );
    BOOST_CHECK( m.getOrThrow(k) == v );
  }

  {
    map_vector<value_t> m2(m);
    BOOST_CHECK( m2.size() == sz );
    m2.insert(m.begin(), m.end());
    BOOST_CHECK( m2.size() == 2 * sz );
    map_vector<value_t>::const_iterator b = m.begin();
    for( map_vector<value_t>::const_iterator it = m2.begin();  it != m2.end();  ++b, ++it )  {
      if( b == m.end() )
        b = m.begin();
      BOOST_CHECK( b->second == it->second );
    }
  }

}

BOOST_AUTO_TEST_SUITE_END()
