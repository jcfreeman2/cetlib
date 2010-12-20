#ifndef CETLIB__REGISTRY_VIA_ID_H
#define CETLIB__REGISTRY_VIA_ID_H

// ======================================================================
//
// registry_via_id<K,V> - a singleton std::map<K,V>
//
// NOTE:
//   V::id() is required to exist and to yield values of type K
//
// ======================================================================

#include "cetlib/exception.h"
#include "cpp0x/type_traits"  // is_same
#include <cassert>
#include <iterator>  // iterator_traits
#include <map>
#include <stdexcept>
#include <utility>   // make_pair

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

  // shorthand:
  typedef  std::map<K const, V>            reg_t;
  typedef  typename reg_t::size_type       size_t;
  typedef  typename reg_t::const_iterator  iter_t;

public:
  typedef  K        key_type;
  typedef  V        value_type;
  typedef  reg_t    collection_type;
  typedef  size_t   size_type;
  typedef  iter_t   const_iterator;

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

#endif  // CETLIB__REGISTRY_VIA_ID_H
