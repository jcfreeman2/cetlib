#ifndef CETLIB_POW_H
#define CETLIB_POW_H

// ======================================================================
//
// pow<>(): minimal-multiply algorithm for integral non-negative powers
//
// ======================================================================

#include "cpp0x/type_traits"

namespace cet {

  template< unsigned N, class T >  inline T  pow( T x );

  template< class T >  inline T  sqr ( T x )  { return pow<2u>(x); }
  template< class T >  inline T  cube( T x )  { return pow<3u>(x); }
}

namespace cet { namespace detail {

  template< unsigned N
          , class    T
          , unsigned = N % 2u
          >
    struct pow;

  template< unsigned N, class T >  struct pow<N,T,0u>;
  template< unsigned N, class T >  struct pow<N,T,1u>;

  template< class T >  struct pow<0u,T,0u>;
  template< class T >  struct pow<1u,T,1u>;
  template< class T >  struct pow<2u,T,0u>;

} }  // cet::detail

// ----------------------------------------------------------------------

template< unsigned N, class T >
inline T
  cet::pow( T x ) { return detail::pow<N,T>()( x ); }

// ----------------------------------------------------------------------

template< unsigned N, class T >
  struct cet::detail::pow<N,T,0u>
{
  pow<N/2u,T> pow_half;
  T  operator () ( T x ) { return pow_half(x * x); }
};  // pow<N,T,0>

template< unsigned N, class T >
  struct cet::detail::pow<N,T,1u>
{
  pow<N-1u,T> pow_even;
  T  operator () ( T x ) { return x * pow_even(x); }
};  // pow<N,T,1>

template< class T >
  struct cet::detail::pow<0u,T,0u>
{
  T  operator () ( T x ) { return static_cast<T>(1); }
};  // pow<0,T,0>

template< class T >
  struct cet::detail::pow<1u,T,1u>
{
  T  operator () ( T x ) { return x; }
};  // pow<1,T,1>

template< class T >
  struct cet::detail::pow<2u,T,0u>
{
  T  operator () ( T x ) { return x * x; }
};  // pow<2,T,0>

// ======================================================================

#endif
