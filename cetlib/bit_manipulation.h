#ifndef CETLIB_BIT_MANIPULATION_H
#define CETLIB_BIT_MANIPULATION_H

// ======================================================================
//
// bit_manipulation: Compile-time bit manipulations
//
// ======================================================================

#include "cpp0x/type_traits"
#include <limits>

// ======================================================================

namespace cet {

  template< class U
          , bool = std::is_unsigned<U>::value
          >
    struct bit_size;

  template< class U >
    struct bit_size<U,true>
  {
    static  size_t const  value = std::numeric_limits<U>::digits;
  };

}  // namespace cet

// ======================================================================

namespace cet {

  template< class U, size_t n
          , bool = n < bit_size<U>::value
          >
    struct bit_number;

  template< class U, size_t n >
    struct bit_number<U,n,true>
  {
    static  size_t const  value = U(1u) << n;
  };

  template< class U, size_t n >
    struct bit_number<U,n,false>
  {
    static  size_t const  value = U(0u);
  };

}  // namespace cet

// ======================================================================

namespace cet {

  template< class U, size_t n
          , bool = std::is_unsigned<U>::value
          , bool = n+1 < bit_size<U>::value
          >
    struct right_bits;

  template< class U, size_t n >
    struct right_bits<U,n,true,true>
  {
    static U const value = bit_number<U,n+1>::value - U(1u);
  };

  template< class U, size_t n >
    struct right_bits<U,n,true,false>
  {
    static U const value = ~0u;
  };

}  // namespace cet

// ======================================================================

namespace cet {

  template< class U, size_t n
          , bool = std::is_unsigned<U>::value
          , bool = n <= bit_size<U>::value
          >
    struct left_bits;

  template< class U, size_t n >
    struct left_bits<U,n,true,true>
  {
  private:
    static U const n_zeros = bit_size<U>::value - n;

  public:
    static U const value = ~ right_bits<U,n_zeros>::value;
  };

  template< class U, size_t n >
    struct left_bits<U,n,true,false>
  {
    static U const value = U(-1);
  };

}  // namespace cet

// ======================================================================

namespace cet {

  template< class U >
    inline
    typename std::enable_if< std::is_unsigned<U>::value
                           , U
                           >::type
    circ_lshift( U X, U n )
  {
    static  size_t const  nbits = bit_size<U>::value;
    n %= nbits;
    return  (X << n)
         |  (X >> (nbits-n));
  }

}  // namespace cet

// ======================================================================

#endif
