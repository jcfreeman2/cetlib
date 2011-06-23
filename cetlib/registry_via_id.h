#ifndef CETLIB_REGISTRY_VIA_ID_H
#define CETLIB_REGISTRY_VIA_ID_H

// ======================================================================
//
// registry_via_id<K,V>: A singleton std::map<K,V> requiring that V::id()
//                       exists and yields values of type K
//
// ======================================================================

#include "cetlib/exception.h"
#include "cpp0x/type_traits"
#include <cassert>
#include <iterator>
#include <map>
#include <stdexcept>
#include <utility>

namespace cet {
  template< class K, class V >
    class registry_via_id;

  namespace detail {
    template< class K, class V, K (V::*)() const = &V::id >
      struct must_have_id  { typedef  K  type; };
  }
}

// ======================================================================

template< class K, class V >
  class cet::registry_via_id
{
  // non-instantiable (and non-copyable, just in case):
  registry_via_id( );
  ~registry_via_id( );
  registry_via_id( registry_via_id const & );
  void  operator = ( registry_via_id const & );

public:
  typedef  std::map<K const, V>                      collection_type;
  typedef  typename collection_type::key_type        key_type;
  typedef  typename collection_type::mapped_type     mapped_type;
  typedef  typename collection_type::value_type      value_type;
  typedef  typename collection_type::size_type       size_type;
  typedef  typename collection_type::const_iterator  const_iterator;

  // observers:
  static  bool
    empty( )  { return the_registry_().empty(); }
  static  size_type
    size( )  { return the_registry_().size(); }

  // iterators:
  static  const_iterator
     begin( )  { return the_registry_().begin(); }
  static  const_iterator
     end( )  { return the_registry_().end(); }

  // mutators:
  static  typename detail::must_have_id<K,V>::type
    put( V const & value );
  template< class FwdIt >
  static  void
    put( FwdIt begin, FwdIt end );
  static  void
    put( collection_type c );

  // accessors:
  static  collection_type const &
    get( ) noexcept  { return the_registry_(); }
  static  V const &
    get( K const & key );
  static  bool
    get( K const & key, V & value ) noexcept;

private:
  // encapsulated singleton:
  static  collection_type &
    the_registry_( )
  {
    static  collection_type  the_registry;
    return the_registry;
  }

};  // registry_via_id<>

// ----------------------------------------------------------------------

template< class K, class V >
typename cet::detail::must_have_id<K,V>::type
  cet::registry_via_id<K,V>::put( V const & value )
{
  K id = value.id();
  the_registry_().insert( std::make_pair(id, value) );
  return id;
}

template< class K, class V >
template< class FwdIt >
void
  cet::registry_via_id<K,V>::put( FwdIt b, FwdIt e )
{
  STATIC_ASSERT( (std::is_same< V
                              , typename std::iterator_traits<FwdIt>::value_type
                              >::value)
               , "Iterator is inconsistent with registry's value_type!"
               );
  for( ; b != e; ++b )
    (void)put(*b);
}


template< class K, class V >
void
  cet::registry_via_id<K,V>::put( collection_type c )
{
  for( const_iterator b = c.begin()
                    , e = c.end(); b != e; ++b )
    (void)put(b->second);
}

// ----------------------------------------------------------------------

template< class K, class V >
V const &
  cet::registry_via_id<K,V>::get( K const & key )
{
  const_iterator it = the_registry_().find(key);
  if( it == the_registry_().end() )
    throw cet::exception("cet::registry_via_id")
      << "Key \"" << key << "\" not found in registry";
  return it->second;
}

template< class K, class V >
bool
  cet::registry_via_id<K,V>::get( K const & key, V & value ) noexcept
try
{
  value = get(key);
  return true;
}
catch( cet::exception const & )
{
  return false;
}

// ======================================================================

#endif
