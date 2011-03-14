#ifndef VALUE_PTR_H
#define VALUE_PTR_H

// ======================================================================
//
// value_ptr - a pointer treating its pointee as-if contained by value
//
// ----------------------------------------------------------------------
//
// This smart pointer template mimics value semantics for its pointee:
//   - the pointee lifetime matches the pointer lifetime, and
//   - the pointee is copied whenever the pointer is copied.
//
// Having such a template provides a standard vocabulary to denote
// such pointers, with no need for further comment or other
// documentation to describe the semantics involved.
//
// As a small bonus, this template's c'tors ensure that all instance
// variables are initialized.
//
// ----------------------------------------------------------------------
//
// Originally inspired by Item 31 in
//   Herb Sutter:  _More Exceptional C++_, Addison-Wesley, 2002.
//   ISBN 0-201-70434-X
// and its predecessor
//   Herb Sutter:  Smart Pointer Members.  GotW #62, undated.
//   http://www.gotw.ca/gotw/062.htm
// and independently by
//   Alan Griffiths:  "Ending with the grin," 1999.
//   URL http://www.octopull.demon.co.uk/arglib/TheGrin.html
// and later by
//   Axter (David Maisonave):  Clone Smart Pointer (clone_ptr). 2005.
//   http://www.codeguru.com/cpp/cpp/algorithms/general/article.php/c10407
//
// ----------------------------------------------------------------------
//
// Alternative names for consideration (shown alphabetically):
//   clone_ptr, cloned_ptr, cloning_ptr, copycat_ptr, copied_ptr,
//   copying_ptr, dup_ptr, duplicate_ptr, duplicating_ptr,
//   matched_ptr, matching_ptr, replicating_ptr, twin_ptr, twinning_ptr
//
// ----------------------------------------------------------------------
//
// Questions:
//   - Should value_ptr be specialized to work with array types a la
//     unique_ptr?
//   - Should value_ptr take an allocator argument in addition to a
//     cloner and a deleter?  (Only the cloner would use the allocator.)
//   - This implementation assumes the cloner and deleter types are
//     stateless; are these viable assumptions?
//   - If cloners and deleters are allowed to be stateful, what policies
//     should apply when they are being copied during a value_ptr copy?
//   - We have operators ==, !=, < ; should <=  >  >=  be supported?
//   - With which, if any, other smart pointers should this template
//     innately interoperate, and to what degree?
//
// ======================================================================

#include "cpp0x/cstddef"
#include "cpp0x/memory"
#include "cpp0x/type_traits"
#include <exception>
#include <utility>

// ======================================================================

namespace cet {

  namespace detail {

    template< class T >
      struct has_clone
    {
    private:
      typedef  char (& yes_t)[1];
      typedef  char (& no_t )[2];

      template< class U, U* (U::*)() = &U::clone >  struct cloneable { };

      template< class U >  static  yes_t  test( cloneable<U>* );
      template< class   >  static  no_t   test( ... );

    public:
      static  bool const  value = sizeof(test<T>(0)) == sizeof(yes_t);
    };  // has_clone<>

  }  // namespace detail

  template< class Element
          , class Pointee = typename std::remove_reference<Element>::type
          , bool = detail::has_clone<Pointee>::value
          >
    class default_clone
  {
    typedef  typename std::add_pointer<Pointee>::type
             pointer;

  public:
    template< class Ptr >
    pointer
      operator () ( Ptr const & p ) const
    { return p->clone(); }

  };  // default_clone<>

  template< class Element, class Pointee >
    class default_clone<Element, Pointee, false>
  {
    typedef  typename std::add_pointer<Pointee>::type
             pointer;

  public:
    template< class Ptr >
    pointer
      operator () ( Ptr const & p ) const
    { return new Pointee( *p ); }

  };  // default_clone<>

  // ======================================================================

  template< class Element
          , class Cloner = default_clone<Element>
          , class Deleter = std::default_delete<Element>
          >
    class value_ptr
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
            , class Q = typename value_ptr<P>::pointer
            >
      struct is_compatible
    : public std::is_convertible< Q, pointer >
    { };

  public:
    // --- default c'tor:
    CONSTEXPR_FCTN
      value_ptr( ) noexcept
      : p( pointer() )
    { }

    // --- copy/move c'tors:
    value_ptr( value_ptr const & other )
      : p( clone_from (other.p) )
    { }
  #ifdef CPP0X_HAS_RVALUE_REFERENCES
    value_ptr( value_ptr && other ) noexcept
      : p( other.release() )
    { }
  #endif

    // --- copy/move assignments:
    value_ptr &
      operator = ( value_ptr const & other ) noexcept
    { value_ptr tmp(other); swap(tmp); return *this; }
  #ifdef CPP0X_HAS_RVALUE_REFERENCES
    value_ptr &
      operator = ( value_ptr && other ) noexcept
    { reset( other.release() ); return *this; }
  #endif

    // --- copy from native pointer, possibly nullptr:
    explicit
      value_ptr( pointer const other ) noexcept
      : p( other )
    { }
    value_ptr &
      operator = ( pointer const other )
    { reset( other ); return *this; }

    // --- copy from auto_ptr<>:
    value_ptr( std::auto_ptr<Element> & other ) :
      p( other.release() )
    { }
    value_ptr &  operator = ( std::auto_ptr<Element> & other )
    { value_ptr tmp(other); swap(tmp); return *this; }

    // --- d'tor:
      ~value_ptr( void ) noexcept
    { reset(); }

    // --- pointer behaviors:
    reference
      operator *  ( ) const noexcept
    { return *get(); }
    pointer
      operator -> ( ) const noexcept
    { return  get(); }
    reference
      at          ( ) const
    { return empty() ? throw std::exception() : *get(); }

    // --- conversions:
    EXPLICIT_CONV_OP
      operator bool    ( ) const noexcept
    { return get(); }
    EXPLICIT_CONV_OP
      operator pointer ( ) const noexcept
    { return get(); }

    // --- smart pointer observing behaviors:
    bool
      empty( ) const noexcept
    { return get() == pointer(); }
    pointer
      get( ) const noexcept
    { return p; }

    // --- smart pointer mutating behaviors:
    pointer
      release( ) noexcept
    { pointer old = get(); p = pointer(); return old; }
    void
      reset( pointer const t = pointer() ) noexcept
    { Deleter()(p); p = t; }
    void
      swap( value_ptr & other ) noexcept
    { std::swap(p, other.p); }

    // --- copying/moving from other value_ptr<>s:
    template< class P >
      value_ptr( value_ptr<P> const & other
               , typename std::enable_if<is_compatible<P>::value>::type * = 0
               )
    : p( clone_from(other.get()) )
    { }
  #ifdef CPP0X_HAS_RVALUE_REFERENCES
    template< class P >
      value_ptr( value_ptr<P> && other
               , typename std::enable_if<is_compatible<P>::value>::type * = 0
               )
    : p( other.release() )
    { }
  #endif
    template< class P >
      typename std::enable_if< is_compatible<P>::value
                             , value_ptr &
                             >::type
      operator = ( value_ptr<P> const & other )
    { reset( clone_from(other.get()) ); return *this; }
  #ifdef CPP0X_HAS_RVALUE_REFERENCES
    template< class P
            , bool = std::is_convertible< typename value_ptr<P>::pointer
                                        , pointer
                                        >::value
            >
      value_ptr &
      operator = ( value_ptr<P> && other )
    { reset( other.release() ); return *this; }
  #endif

    // --- comparisons:
    template< class P >
      typename std::enable_if< is_compatible<P>::value
                             , bool
                             >::type
      operator == ( value_ptr<P> const & other )
    { return get() == other.get(); }
    template< class P >
      typename std::enable_if< is_compatible<P>::value
                             , bool
                             >::type
      operator != ( value_ptr<P> const & other )
    { return ! operator == (other); }
    template< class P >
      typename std::enable_if< is_compatible<P>::value
                             , bool
                             >::type
      operator < ( value_ptr<P> const & other )
    { return get() < other.get(); }

    // --- additional interoperation with nullptr_t:
    value_ptr &
      operator = ( std::nullptr_t )
    { reset(); return *this; }
    bool
      operator == ( std::nullptr_t )
    { return empty(); }
    bool
      operator != ( std::nullptr_t )
    { return ! empty(); }

  private:
    pointer  p;

    template< class P >
      pointer
      clone_from( P const p ) const
    { return p ? Cloner()(p) : pointer(); }

  };  // value_ptr<>


  // --- provide commutative (in)equality with nullptr_t:
  template< class Element >
    bool
    operator == ( std::nullptr_t, value_ptr<Element> const & other )
  { return other.empty(); }
  template< class Element >
    bool
    operator != ( std::nullptr_t, value_ptr<Element> const & other )
  { return ! other.empty(); }

  // --- non-member swap:
  template< class Element >
    void
    swap( value_ptr<Element> & x, value_ptr<Element> & y ) noexcept
  { x.swap(y); }

}  // namespace cet

// ======================================================================

#endif
