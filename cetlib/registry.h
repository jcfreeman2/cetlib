#ifndef CETLIB__REGISTRY_H
#define CETLIB__REGISTRY_H

// ======================================================================
//
// registry<K,V> - a singleton std::map<K,V>
//
// ======================================================================


#include "cetlib/exception.h"
#include <map>
#include <stdexcept>
#include <utility>


namespace cet {
  template< class K, class V >  class registry;
}


// ======================================================================


template< class K, class V >
  class cet::registry
{
  // non-instantiable (and non-copyable, just in case):
  registry( );
  ~registry( );
  registry( registry const & );
  void  operator = ( registry const & );

  // shorthand:
  typedef  std::map<K const, V>            reg_t;
  typedef  typename reg_t::const_iterator  iter_t;

public:
  static  bool
    empty( )  { return the_registry_().empty(); }
  static  typename reg_t::size_type
    size( )  { return the_registry_().size(); }

  static  void
    put( K const & key, V const & value );

  static  V const &
    get( K const & key );
  static  bool
    get( K const & key, V & value ) throw();

private:
  // encapsulated singletons:
  static  reg_t &
    the_registry_( )
  {
    static  reg_t  the_registry;
    return the_registry;
  }

  static  void
    throw_nonesuch_( )
  {
    static  cet::exception  nonesuch("cet::registry", "Not found");
    throw nonesuch;
  }

};  // registry<>


// ----------------------------------------------------------------------
// out-of-line code

template< class K, class V >
void
  cet::registry<K,V>::put( K const & key, V const & value )
{ the_registry_().insert( std::make_pair(key, value) ); }

template< class K, class V >
V const &
  cet::registry<K,V>::get( K const & key )
{
  iter_t it = the_registry_().find(key);
  if( it == the_registry_().end() )
    throw_nonesuch_();
  return it->second;
}

template< class K, class V >
bool
  cet::registry<K,V>::get( K const & key, V & value ) throw()
{
  iter_t it = the_registry_().find(key);
  return it == the_registry_().end() ?  (                   false)
                                     :  (value = it->second, true);
}


// ======================================================================


#endif  // CETLIB__REGISTRY_H
