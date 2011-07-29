#ifndef CETLIB_EXEMPT_PTR_H
#define CETLIB_EXEMPT_PTR_H

// ======================================================================
//
// exempt_ptr: A pointer taking no notice of its pointee's lifetime
//             (thus exempt from any role in managing same, and thus
//             copyable with no impact on its pointee)
//
// ----------------------------------------------------------------------
//
// This (not very) smart pointer template is intended as a replacement
// for bare/native/raw/built-in C++ pointers, especially when used to
// communicate with (say) legacy code that traffics in such pointers.
//
// Having such a template provides a standard vocabulary to denote
// non-owning pointers, with no need for further comment or other
// documentation to describe the semantics involved.
//
// As a small bonus, this template's c'tors ensure that all instance
// variables are initialized.
//
// ----------------------------------------------------------------------
//
// Alternative names for consideration (shown alphabetically):
//   aloof_ptr, agnostic_ptr, apolitical_ptr, ascetic_ptr, attending_ptr,
//   austere_ptr, bare_ptr, blameless_ptr, classic_ptr,
//   disinterested_ptr, disowned_ptr, disowning_ptr, dumb_ptr,
//   emancipated_ptr, estranged_ptr, excused_ptr,
//   faultless_ptr, free_ptr, freeagent_ptr, guiltless_ptr, handsoff_ptr,
//   ignorant_ptr, impartial_ptr, independent_ptr, innocent_ptr,
//   just_a_ptr, legacy_ptr,
//   naked_ptr, neutral_ptr, nonown_ptr, nonowning_ptr, notme_ptr,
//   oblivious_ptr, observer_ptr, observing_ptr, open_ptr, ownerless_ptr,
//   pointer, ptr, pure_ptr, quintessential_ptr,
//   severe_ptr, simple_ptr, stark_ptr, straight_ptr,
//   true_ptr, unmanaged_ptr, unowned_ptr, untainted_ptr, unyoked_ptr,
//   virgin_ptr, visiting_ptr
//
// ----------------------------------------------------------------------
//
// Questions:
//   - We have operators ==, !=, < ; should <=  >  >=  be supported?
//   - With which, if any, other smart pointers should this template
//     innately interoperate, and to what degree?
//
// ======================================================================

#include "cpp0x/cstddef"
#include "cpp0x/type_traits"
#include <exception>
#include <utility>

namespace cet {
  template< class Element >
    class exempt_ptr;

  template< class Element >
  void
    swap( exempt_ptr<Element> &, exempt_ptr<Element> & ) noexcept;
}

// ======================================================================

template< class Element >
  class cet::exempt_ptr
{
public:
  // --- publish our type parameter and variations thereof:
  typedef  Element
           element_type;
  typedef  typename std::remove_reference<element_type>::type
           pointee;
  typedef  typename std::add_pointer<pointee>::type
           pointer;
  typedef  typename std::add_lvalue_reference<pointee>::type
           reference;

private:
  template< class P
          , class Q = typename exempt_ptr<P>::pointer
          >
    struct is_compatible
  : public std::is_convertible< Q, pointer >
  { };

public:
  // --- default c'tor:
  CONSTEXPR_FCTN
    exempt_ptr( ) noexcept
  : p( pointer() )
  { }

  // --- use compiler-generated copy c'tor, copy assignment, and d'tor

  // --- copying from native pointers, possibly nullptr:
  template< class P >
  CONSTEXPR_FCTN
    exempt_ptr( P other
              , typename std::enable_if< std::is_convertible< P, pointer
                                                            >::value
                                       >::type * = 0
              ) noexcept
  : p( other )
  { }
  template< class P >
    typename std::enable_if< std::is_convertible<P,pointer>::value
                           , exempt_ptr &
                           >::type
    operator = ( P other ) noexcept
  { reset( other ); return *this; }

  // --- copying from other exempt_ptr<>s:
  template< class P >
    exempt_ptr( exempt_ptr<P> const & other
              , typename std::enable_if<is_compatible<P>::value>::type * = 0
              ) noexcept
  : p( other.get() )
  { }
  template< class P >
    typename std::enable_if< is_compatible<P>::value
                           , exempt_ptr &
                           >::type
    operator = ( exempt_ptr<P> const & other ) noexcept
  { reset( other.get() ); return *this; }

  // --- pointer behaviors:
  reference
    operator *  ( ) const noexcept  { return *get(); }
  pointer
    operator -> ( ) const noexcept  { return  get(); }
  reference
    at          ( ) const
  { return empty() ? throw std::exception() : *get(); }

  // --- conversions:
#ifdef CPP0X_HAS_EXPLICIT_CONVERSION_OPERATORS
  explicit
    operator bool    ( ) const noexcept  { return get(); }
#else
private:
  struct _safe_ { int _bool_; };
public:
    operator int _safe_::* ( ) const noexcept
  { return get() ? & _safe_::_bool_ : 0; }
#endif  // CPP0X_HAS_EXPLICIT_CONVERSION_OPERATORS

  // --- smart pointer observing behaviors:
  bool
    empty( ) const noexcept  { return get() == pointer(); }
  pointer
    get( ) const noexcept  { return p; }

  // --- smart pointer mutating behaviors:
  pointer
    release( ) noexcept { pointer old = get(); reset(); return old; }
  void
    reset( pointer t = pointer() ) noexcept  { p = t; }
  void
    swap( exempt_ptr & other ) noexcept  { std::swap(p, other.p); }

  // --- comparisons:
  template< class P >
    typename std::enable_if< is_compatible<P>::value
                           , bool
                           >::type
    operator == ( exempt_ptr<P> const & other ) noexcept
  { return get() == other.get(); }
  template< class P >
    typename std::enable_if< is_compatible<P>::value
                           , bool
                           >::type
    operator != ( exempt_ptr<P> const & other ) noexcept
  { return ! operator == (other); }
  template< class P >
    typename std::enable_if< is_compatible<P>::value
                           , bool
                           >::type
    operator < ( exempt_ptr<P> const & other ) noexcept
  { return get() < other.get(); }

private:
  pointer  p;

};  // exempt_ptr<>

// ----------------------------------------------------------------------

// --- non-member swap:
template< class Element >
inline void
  cet::swap( exempt_ptr<Element> & x, exempt_ptr<Element> & y ) noexcept
{ x.swap(y); }

// ======================================================================

#endif
