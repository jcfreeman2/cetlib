#ifndef CETLIB_ZERO_INIT_H
#define CETLIB_ZERO_INIT_H

// ======================================================================
//
// zero_init: wrap a value of native arithmetic or pointer type T,
//            ensuring a default-initialized value of T(0)
//
// ======================================================================

#include "cpp0x/type_traits"

namespace cet {
  template< class T
          , class = typename std::enable_if<  std::is_arithmetic<T>::value
                                           || std::is_pointer   <T>::value
                                           >::type
          >
    class zero_init;
}

// ----------------------------------------------------------------------

template< class T, class >
  class cet::zero_init
{
  typedef  typename std::conditional< std::is_const<T>::value
                                    , T
                                    , T &
                                    >::type
           T_ref;

public:
  zero_init( )       : val( static_cast<T>(0) )  { }
  zero_init( T val ) : val( val               )  { }

#if defined __GXX_EXPERIMENTAL_CXX0X__
  template< class U
          , class = typename std::enable_if< std::is_convertible<U,T>::value
                                           >::type
          >
    zero_init( zero_init<U> const & other )
#else
  template< class U >
    zero_init( zero_init<U> const & other
             , typename std::enable_if< std::is_convertible<U,T>::value
                                      >::type * = 0
             )
#endif
  : val( static_cast<T>(other) )
  { }

  // use compiler-generated copy c'tor, copy assignment, and d'tor

  operator T_ref     ( )        { return val; }
  operator T const & ( ) const  { return val; }

private:
  T val;

};  // zero_init<T>

// ======================================================================

#endif
