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
#include <map>
#include <stdexcept>
#include <utility>

namespace cet {
  template< class K, class V >  class registry_via_id;
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

  static  bool
    empty( )  { return the_registry_().empty(); }
  static  size_t
    size( )  { return the_registry_().size(); }

  static  const_iterator
     begin( )  { return the_registry_().begin(); }
  static  const_iterator
     end( )  { return the_registry_().end(); }

  static  K
    put( V const & value );

  static  V const &
    get( K const & key );
  static  bool
    get( K const & key, V & value ) throw();

private:
  // encapsulated singleton:
  static  reg_t &
    the_registry_( )
  {
    static  reg_t  the_registry;
    return the_registry;
  }

};  // registry_via_id<>

// ----------------------------------------------------------------------

template< class K, class V >
K
  cet::registry_via_id<K,V>::put( V const & value )
{
  the_registry_().insert( std::make_pair(value.id(), value) );
  return value.id();
}

template< class K, class V >
V const &
  cet::registry_via_id<K,V>::get( K const & key )
{
  iter_t it = the_registry_().find(key);
  if( it == the_registry_().end() )
    throw cet::exception("cet::registry_via_id")
      << "Key \"" << key << "\" not found in registry";
  return it->second;
}

template< class K, class V >
bool
  cet::registry_via_id<K,V>::get( K const & key, V & value ) throw()
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
