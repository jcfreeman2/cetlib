#ifndef VALUE_PTR_H
#define VALUE_PTR_H

// ======================================================================
//
// value_ptr: A pointer treating its pointee as-if contained by value
//
// ----------------------------------------------------------------------
//
// This smart pointer template mimics value semantics for its pointee:
//   - the pointee lifetime matches the pointer lifetime, and
//   - the pointee is copied whenever the pointer is copied.
//
// Having such a template provides a standard vocabulary to denote such
// pointers, with no need for further comment or other documentation to
// describe the semantics involved.
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
#include "cpp0x/functional"
#include "cpp0x/memory"
#include "cpp0x/type_traits"
#include <exception>
#include <utility>

namespace cet {
  namespace _ {
    template< class T >  struct has_clone;
  }

  template< class Element
          , class Pointee = typename std::remove_reference<Element>::type
          , bool          = _::has_clone<Pointee>::value
          >
    class default_clone;
  template< class Element, class Pointee >
    class default_clone<Element, Pointee, false>;

  template< class Element
          , class Cloner  = default_clone<Element>
          , class Deleter = std::default_delete<Element>
          >
    class value_ptr;

  template< class E1, class C, class D, class E2 >
  void
    swap( value_ptr<E1,C,D> &, value_ptr<E2,C,D> & ) noexcept;

  template< class E1, class C, class D, class E2 >
  bool
    operator == ( value_ptr<E1,C,D> const &, value_ptr<E2,C,D> const & );
  template< class E1, class C, class D, class E2 >
  bool
    operator != ( value_ptr<E1,C,D> const &, value_ptr<E2,C,D> const & );

  template< class E1, class C, class D, class E2 >
  bool
    operator < ( value_ptr<E1,C,D> const &, value_ptr<E2,C,D> const & );
  template< class E1, class C, class D, class E2 >
  bool
    operator > ( value_ptr<E1,C,D> const &, value_ptr<E2,C,D> const & );
  template< class E1, class C, class D, class E2 >
  bool
    operator <= ( value_ptr<E1,C,D> const &, value_ptr<E2,C,D> const & );
  template< class E1, class C, class D, class E2 >
  bool
    operator >= ( value_ptr<E1,C,D> const &, value_ptr<E2,C,D> const & );
}

// ======================================================================

template< class T >
  struct cet::_::has_clone
{
private:
  typedef  char (& yes_t)[1];
  typedef  char (& no_t )[2];

  template< class U, U* (U::*)() const = &U::clone >  struct cloneable { };

  template< class U >  static  yes_t  test( cloneable<U>* );
  template< class   >  static  no_t   test( ... );

public:
  static  bool const  value = sizeof(test<T>(0)) == sizeof(yes_t);
};  // has_clone<>

// ----------------------------------------------------------------------

template< class Element, class Pointee, bool >
  class cet::default_clone
{
  typedef  typename std::add_pointer<Pointee>::type
           pointer;

public:
  template< class Ptr >
  typename  std::enable_if< std::is_convertible<Ptr, pointer>::value
                          , pointer
                          >::type
    operator () ( Ptr const & p ) const
  { return p->clone(); }

};  // default_clone<>

template< class Element, class Pointee >
  class cet::default_clone<Element, Pointee, false>
{
  typedef  typename std::add_pointer<Pointee>::type
           pointer;

public:
  template< class Ptr >
  typename  std::enable_if< std::is_convertible<Ptr, pointer>::value
                          , pointer
                          >::type
    operator () ( Ptr const & p ) const
  { return new Pointee( *p ); }

};  // default_clone<>

// ----------------------------------------------------------------------

template< class Element, class Cloner, class Deleter >
  class cet::value_ptr
{
public:
  // --- publish our template parameters and variations thereof:
  typedef  Element                                            element_type;
  typedef  Cloner                                             cloner_type;
  typedef  Deleter                                            deleter_type;
  typedef  typename std::add_pointer<Element>::type           pointer;
    // TODO: use Deleter::pointer, if it exists, in lieu of above
  typedef  typename std::add_lvalue_reference<Element>::type  reference;

private:
  template< class P
          , class Q = typename value_ptr<P>::pointer
          >
    struct is_compatible
    : public std::is_convertible< Q, pointer >
  { };

public:
  // default c'tor:
  CONSTEXPR_FCTN  value_ptr( ) noexcept;

  // ownership-taking c'tors:
  CONSTEXPR_FCTN  value_ptr( std::nullptr_t ) noexcept;
  explicit        value_ptr( pointer        ) noexcept;

  // copying c'tors:
  explicit  value_ptr( value_ptr const & );
  template< class E >
    explicit  value_ptr( value_ptr<E,Cloner,Deleter> const & );

#ifdef CPP0X_HAS_RVALUE_REFERENCES
  // moving c'tors:
  value_ptr( value_ptr && ) noexcept;
  template< class E >
    value_ptr( value_ptr<E,Cloner,Deleter> && ) noexcept;
#endif // CPP0X_HAS_RVALUE_REFERENCES

  // d'tor:
  ~value_ptr( ) noexcept;

  // copy assignments:
  value_ptr &  operator = ( std::nullptr_t ) noexcept;
  value_ptr &  operator = ( value_ptr const & );
  template< class E >
    value_ptr &  operator = ( value_ptr<E,Cloner,Deleter> const & );

#ifdef CPP0X_HAS_RVALUE_REFERENCES
  // move assignments:
  value_ptr &  operator = ( value_ptr && );
  template< class E >
    value_ptr &  operator = ( value_ptr<E,Cloner,Deleter> && );
#endif // CPP0X_HAS_RVALUE_REFERENCES

  // observers:
  reference             operator *  ( ) const;
  pointer               operator -> ( ) const noexcept;
  pointer               get         ( ) const noexcept;
#ifdef CPP0X_HAS_EXPLICIT_CONVERSION_OPERATORS
  explicit  operator bool ( ) const noexcept;
#else
private:
  struct _safe_ { int _bool_; };
public:
  operator int _safe_::* ( ) const noexcept;
#endif  // CPP0X_HAS_EXPLICIT_CONVERSION_OPERATORS

  // modifiers:
  pointer  release( ) noexcept;
  void     reset( pointer = pointer() ) noexcept;
  void     swap( value_ptr & ) noexcept;
  template< class E >
    void   swap( value_ptr<E,Cloner,Deleter> & ) noexcept;

private:
  pointer       p;

  template< class P >
    pointer
    clone_from( P const p ) const
  { return p ? Cloner()(p) : nullptr; }


#if 0
  // --------------------------------------------------

  // --- copy from auto_ptr<>:
  value_ptr( std::auto_ptr<Element> & other ) :
    p( other.release() )
  { }
  value_ptr &  operator = ( std::auto_ptr<Element> & other )
  { value_ptr tmp(other); swap(tmp); return *this; }

  reference
    at          ( ) const
  { return empty() ? throw std::exception() : *get(); }

  bool
    empty( ) const noexcept
  { return p == nullptr; }

#ifdef CPP0X_HAS_RVALUE_REFERENCES
  template< class P >
    value_ptr( value_ptr<P> && other
             , typename std::enable_if<is_compatible<P>::value>::type * = 0
             )
  : p( other.release() )
  { }
#endif  // CPP0X_HAS_RVALUE_REFERENCES
#endif  // 0

  // --------------------------------------------------

};  // value_ptr<>

// ======================================================================
// member functions:

// ----------------------------------------------------------------------
// default c'tor:

template< class E, class C, class D >
CONSTEXPR_FCTN
  cet::value_ptr<E,C,D>::
  value_ptr( ) noexcept
: p( pointer() )
{ }

// ----------------------------------------------------------------------
// ownership-taking c'tors:

template< class E, class C, class D >
CONSTEXPR_FCTN
  cet::value_ptr<E,C,D>::
  value_ptr( std::nullptr_t ) noexcept
: p( pointer() )
{ }

template< class E, class C, class D >
  cet::value_ptr<E,C,D>::
  value_ptr( pointer const other ) noexcept
: p( other )
{ }

// ----------------------------------------------------------------------
// copying c'tors:

template< class E, class C, class D >
  cet::value_ptr<E,C,D>::
  value_ptr( value_ptr const & other )
: p( clone_from (other.p) )
{ }


template< class E1, class C, class D >
template< class E2 >
  cet::value_ptr<E1,C,D>::
  value_ptr( value_ptr<E2,C,D> const & other )
: p( clone_from (other.p) )
{ }

// ----------------------------------------------------------------------
// moving c'tors:

#ifdef CPP0X_HAS_RVALUE_REFERENCES

template< class E, class C, class D >
  cet::value_ptr<E,C,D>::
  value_ptr( value_ptr && other ) noexcept
: p( other.release() )
{ }

template< class E1, class C, class D >
template< class E2 >
  cet::value_ptr<E1,C,D>::
  value_ptr( value_ptr<E2,C,D> && other ) noexcept
: p( other.release() )
{ }

#endif // CPP0X_HAS_RVALUE_REFERENCES

// ----------------------------------------------------------------------
// d'tor:

template< class E, class C, class D >
  cet::value_ptr<E,C,D>::
  ~value_ptr( ) noexcept
{
  reset();
}

// ----------------------------------------------------------------------
// copy assignments:

template< class E, class C, class D >
cet::value_ptr<E,C,D> &
  cet::value_ptr<E,C,D>::
  operator = ( std::nullptr_t ) noexcept
{
  reset( nullptr );
  return *this;
}

template< class E, class C, class D >
cet::value_ptr<E,C,D> &
  cet::value_ptr<E,C,D>::
    operator = ( value_ptr const & other )
{
  value_ptr tmp(other);
  swap(tmp);
  return *this;
}

template< class E, class C, class D >
template< class E2 >
cet::value_ptr<E,C,D> &
  cet::value_ptr<E,C,D>::
  operator = ( value_ptr<E2,C,D> const & other )
{
  reset( clone_from(other.p) );
  return *this;
}

// ----------------------------------------------------------------------
// move assignments:

#ifdef CPP0X_HAS_RVALUE_REFERENCES

template< class E, class C, class D >
cet::value_ptr<E,C,D> &
  cet::value_ptr<E,C,D>::
  operator = ( value_ptr && other )
{
  reset( other.release() );
  return *this;
}

template< class E, class C, class D >
template< class E2 >
cet::value_ptr<E,C,D> &
  cet::value_ptr<E,C,D>::
  operator = ( value_ptr<E2,C,D> && other )
{
  reset( other.release() );
  return *this;
}

#endif // CPP0X_HAS_RVALUE_REFERENCES

// ----------------------------------------------------------------------
// observers:

template< class E, class C, class D >
typename cet::value_ptr<E,C,D>::reference
  cet::value_ptr<E,C,D>::
  operator * ( ) const
{
  return *get();
}

template< class E, class C, class D >
typename cet::value_ptr<E,C,D>::pointer
  cet::value_ptr<E,C,D>::
  operator -> ( ) const noexcept
{
  return get();
}

template< class E, class C, class D >
typename cet::value_ptr<E,C,D>::pointer
  cet::value_ptr<E,C,D>::
  get( ) const noexcept
{
  return p;
}

template< class E, class C, class D >
  cet::value_ptr<E,C,D>::
#ifdef CPP0X_HAS_EXPLICIT_CONVERSION_OPERATORS
  operator bool ( ) const noexcept
{
  return get();
}
#else
  operator int _safe_::* ( ) const noexcept
{
  return get() ? & _safe_::_bool_ : 0;
}
#endif  // CPP0X_HAS_EXPLICIT_CONVERSION_OPERATORS

// ----------------------------------------------------------------------
// modifiers:

template< class E, class C, class D >
typename cet::value_ptr<E,C,D>::pointer
  cet::value_ptr<E,C,D>::
  release( ) noexcept
{
  pointer old = p;
  p = nullptr;
  return old;
}

template< class E, class C, class D >
void
  cet::value_ptr<E,C,D>::
  reset( pointer const t ) noexcept
{
  D()(p);
  p = t;
}

template< class E, class C, class D >
void
  cet::value_ptr<E,C,D>::
  swap( value_ptr & other ) noexcept
{
  std::swap(p, other.p);
}

template< class E1, class C, class D >
template< class E2 >
void
  cet::value_ptr<E1,C,D>::
  swap( value_ptr<E2,C,D> & other ) noexcept
{
  std::swap(p, other.p);
}

// ======================================================================
// non-member functions:

// ----------------------------------------------------------------------
// non-member swap:

template< class E1, class C, class D, class E2 >
  void
  cet::swap( value_ptr<E1,C,D> & x, value_ptr<E2,C,D> & y ) noexcept
{
  x.swap(y);
}

// ----------------------------------------------------------------------
// non-member (in)equality comparison:

template< class E1, class C, class D, class E2 >
bool
  cet::operator == ( value_ptr<E1,C,D> const & x, value_ptr<E2,C,D> const & y )
{
  return x.get() == y.get();
}

template< class E1, class C, class D, class E2 >
bool
  cet::operator != ( value_ptr<E1,C,D> const & x, value_ptr<E2,C,D> const & y )
{
  return ! operator == (x, y);
}

// ----------------------------------------------------------------------
// non-member ordering:

template< class E1, class C, class D, class E2 >
bool
  cet::operator < ( value_ptr<E1,C,D> const & x, value_ptr<E2,C,D> const & y )
{
  typedef  typename std::common_type< typename value_ptr<E1,C,D>::pointer
                                    , typename value_ptr<E2,C,D>::pointer
                                    >::type
           CT;
  return std::less<CT>()( x.get(), y.get() );
}

template< class E1, class C, class D, class E2 >
bool
  cet::operator > ( value_ptr<E1,C,D> const & x, value_ptr<E2,C,D> const & y )
{
  return y < x;
}

template< class E1, class C, class D, class E2 >
bool
  cet::operator <= ( value_ptr<E1,C,D> const & x, value_ptr<E2,C,D> const & y )
{
  return ! (y < x);
}

template< class E1, class C, class D, class E2 >
bool
  cet::operator >= ( value_ptr<E1,C,D> const & x, value_ptr<E2,C,D> const & y )
{
  return ! (x < y);
}

// ======================================================================

#endif
