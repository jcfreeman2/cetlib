#ifndef CETLIB_MAP_VECTOR_H
#define CETLIB_MAP_VECTOR_H

// ======================================================================
//
// map_vector: A map mimicking a sparse vector interface
//
// Integer subscripting is unsupported and yields a compilation failure.
//
// ======================================================================

#include "cetlib/exception.h"
#include "cpp0x/algorithm"
#include <iosfwd>
#include <vector>

namespace cet  {
  class map_vector_key;

  bool  operator == ( map_vector_key const &, map_vector_key const & );
  bool  operator != ( map_vector_key const &, map_vector_key const & );
  bool  operator <  ( map_vector_key const &, map_vector_key const & );
  bool  operator >  ( map_vector_key const &, map_vector_key const & );
  bool  operator <= ( map_vector_key const &, map_vector_key const & );
  bool  operator >= ( map_vector_key const &, map_vector_key const & );

  std::ostream &  operator << ( std::ostream &, map_vector_key const & );

  template< class Value >  class map_vector;
}

// ======================================================================

class cet::map_vector_key
{
public:
  // c'tors:
  map_vector_key( )                            : key_(-1)   { }
  explicit map_vector_key( int           key );
  explicit map_vector_key( unsigned      key ) : key_(key)  { }
  explicit map_vector_key( unsigned long key );

  // use compiler-generated copy c'tor, copy assignment, and d'tor

  // observers:
  unsigned  asInt ( ) const  { return key_; }
  unsigned  asUint( ) const  { return key_; }
  void ensure_valid() const;

private:
  unsigned key_;

};  // map_vector_key

inline
cet::map_vector_key::map_vector_key( int key )
  : key_(key)
{
  if (key < 0) {
    throw cet::exception("InvalidKey")
      << "Negative key " << key
      << " not valid for map_vector_key.";
  }
}

inline
cet::map_vector_key::map_vector_key( unsigned long key )
  : key_(key)
{
  if (key != key_) {
    throw cet::exception("InvalidKey")
      << "Key " << key
      << " too large for map_vector_key.";
  }
}

inline
void
cet::map_vector_key::ensure_valid() const {
  if (key_ == static_cast<unsigned>(-1)) { // Invalid key
    throw cet::exception("InvalidKey")
      << "Attempt to use an invalid cet::map_vector_key.";
  }
}

// ======================================================================

template< class Value >
  class cet::map_vector
{
public:
  // types:
  typedef  map_vector_key                              key_type;
  typedef  Value                                       mapped_type;
  typedef  std::pair<key_type,mapped_type>             value_type;
  typedef  std::vector<value_type>                     impl_type;

  typedef  typename impl_type::size_type               size_type;
  typedef  typename impl_type::difference_type         difference_type;

  typedef  typename impl_type::iterator                iterator;
  typedef  typename impl_type::const_iterator          const_iterator;
  typedef  typename impl_type::reverse_iterator        reverse_iterator;
  typedef  typename impl_type::const_reverse_iterator  const_reverse_iterator;

  typedef  typename impl_type::allocator_type          allocator_type;
  typedef  typename allocator_type::pointer            pointer;
  typedef  typename allocator_type::const_pointer      const_pointer;
  typedef  typename allocator_type::reference          reference;
  typedef  typename allocator_type::const_reference    const_reference;

  // c'tors:
  map_vector( ) : v_()  { }

  template< class InIter >
    map_vector( InIter b, InIter e ) : v_()  { insert(b, e); }

  // use compiler-generated copy c'tor, copy assignment, and d'tor

  // properties:
  bool       empty( )    const  { return v_.empty();    }
  size_type  size( )     const  { return v_.size();     }
  size_type  max_size( ) const  { return v_.max_size(); }
  size_type  capacity( ) const  { return v_.capacity(); }

  allocator_type  get_allocator( ) const  { return v_.get_allocator(); }

  // observers:
  value_type  front( ) const;
  value_type  back ( ) const;

  size_t  delta( ) const  { return v_.empty() ? 0 : 1 + v_.back().first.asInt(); }

  bool  has( key_type key ) const;

  iterator        find( key_type key );
  const_iterator  find( key_type key ) const;

  mapped_type       *   getOrNull( key_type key );
  mapped_type const *   getOrNull( key_type key ) const;

  mapped_type       &   getOrThrow( key_type key );
  mapped_type const &   getOrThrow( key_type key ) const;

  mapped_type       &  operator [] ( key_type key );
  mapped_type const &  operator [] ( key_type key ) const  { return getOrThrow(key); }
  mapped_type const &  at          ( key_type key ) const  { return getOrThrow(key); }

  // iterators:
  iterator        begin( )        { return v_.begin(); };
  const_iterator  begin( ) const  { return v_.begin(); }

  iterator        end( )        { return v_.end(); };
  const_iterator  end( ) const  { return v_.end(); }

  reverse_iterator        rbegin( )        { return v_.rbegin(); };
  const_reverse_iterator  rbegin( ) const  { return v_.rbegin(); }

  reverse_iterator        rend( )        { return v_.rend(); };
  const_reverse_iterator  rend( ) const  { return v_.rend(); }

  // mutators:
  void  clear( )  { v_.clear(); }

  void  reserve( size_type n )  { v_.reserve(n); }

  void  swap( map_vector<mapped_type> & other )  { v_.swap(other.v_); }

  void  push_back( value_type const & x );

  template< class InIter >
    void  insert( InIter b, InIter e );

  // MUST UPDATE WHEN CLASS IS CHANGED!
  static short  Class_Version() { return 10; }

private:
  impl_type  v_;

  bool  class_invariant( ) const;

  static  bool  lt ( value_type const &, value_type const & );

};  // map_vector<>

// ======================================================================
// additional map_vector_key implementation

// ----------------------------------------------------------------------
// comparisons:

inline bool
  cet::operator == ( map_vector_key const & k1, map_vector_key const & k2 )
{ return k1.asInt() == k2.asInt(); }

inline bool
  cet::operator != ( map_vector_key const & k1, map_vector_key const & k2 )
{ return k1.asInt() != k2.asInt(); }

inline bool
  cet::operator <  ( map_vector_key const & k1, map_vector_key const & k2 )
{ return k1.asInt() < k2.asInt(); }

inline bool
  cet::operator >  ( map_vector_key const & k1, map_vector_key const & k2 )
{ return k1.asInt() > k2.asInt(); }

inline bool
  cet::operator <= ( map_vector_key const & k1, map_vector_key const & k2 )
{ return k1.asInt() <= k2.asInt(); }

inline bool
  cet::operator >= ( map_vector_key const & k1, map_vector_key const & k2 )
{ return k1.asInt() >= k2.asInt(); }

// ----------------------------------------------------------------------
// output:

inline std::ostream &
  cet::operator << ( std::ostream & os, cet::map_vector_key const & key )
{ return os << key.asInt(); }

// ======================================================================
// additional map_vector<> implementation

// ----------------------------------------------------------------------
// observers:

template< class Value >
typename cet::map_vector<Value>::value_type
  cet::map_vector<Value>::
  front( ) const
{
  return v_.empty()
       ? throw cet::exception("map_vector::front") << "container is empty!\n"
       : v_.front();
}

template< class Value >
typename cet::map_vector<Value>::value_type
  cet::map_vector<Value>::
  back ( ) const
{
  return v_.empty()
       ? throw cet::exception("map_vector::back") << "container is empty!\n"
       : v_.back();
}

template< class Value >
bool
  cet::map_vector<Value>::
  has( key_type key ) const
{
  value_type  v(key, mapped_type());
  return std::binary_search(v_.begin(), v_.end(), v, lt );
}

template< class Value >
typename cet::map_vector<Value>::iterator
  cet::map_vector<Value>::
  find( key_type key )
{
  value_type  v(key, mapped_type());

  iterator const begin = v_.begin()
               , end   = v_.end();

  iterator  it = std::lower_bound(begin, end, v, lt);
  if( it != end  &&  it->first != key )
    it = end;

  return it;
}

template< class Value >
typename cet::map_vector<Value>::const_iterator
  cet::map_vector<Value>::
  find( key_type key ) const
{
  value_type  v(key, mapped_type());

  const_iterator const begin = v_.begin()
                     , end   = v_.end();

  const_iterator  it = std::lower_bound(begin, end, v, lt);
  if( it != end  &&  it->first != key )
    it = end;

  return it;
}

template< class Value >
Value *
  cet::map_vector<Value>::
  getOrNull( key_type key )
{
  iterator  it = find(key);
  return it == v_.end()  ?  nullptr  :  & it->second;
}

template< class Value >
Value const *
  cet::map_vector<Value>::
  getOrNull( key_type key ) const
{
  const_iterator  it = find(key);
  return it == v_.end()  ?  nullptr  :  & it->second;
}

template< class Value >
Value &
  cet::map_vector<Value>::
  getOrThrow( key_type key )
{
  Value *  p = getOrNull(key);
  if( p == nullptr )
    throw cet::exception("map_vector::getOrThrow")
      << "out of range (no such key): " << key.asInt()
      << std::endl;

  return *p;
}

template< class Value >
Value const &
  cet::map_vector<Value>::
  getOrThrow( key_type key ) const
{
  Value const *  p = getOrNull(key);
  if( p == nullptr )
    throw cet::exception("map_vector::getOrThrow")
      << "out of range (no such key): " << key.asInt()
      << std::endl;

  return *p;
}

template< class Value >
Value &
  cet::map_vector<Value>::
  operator [] ( key_type key )
{
  value_type  v(key, mapped_type());

  iterator const begin = v_.begin()
               , end   = v_.end();

  iterator  it = std::lower_bound(begin, end, v, lt);
  if( it == end  ||  it->first != key )
    it = v_.insert(it, v);

  return it->second;
}

// ----------------------------------------------------------------------
// mutators:

template< class Value >
void
  cet::map_vector<Value>::
  push_back( value_type const & x )
{
  x.first.ensure_valid();
  v_.push_back( std::make_pair(x.first.asInt() + delta(), x.second) );
}

template< class Value >
template< class InIter >
void
  cet::map_vector<Value>::
  insert( InIter b, InIter e )
{
  size_t d = delta();
  for(  ;  b != e;  ++b ) {
    b->first.ensure_valid();
    v_.push_back( std::make_pair(b->first.asInt() + d, b->second) );
  }
}

// ----------------------------------------------------------------------
// helpers:

template< class Value >
bool
  cet::map_vector<Value>::
  class_invariant( ) const
{
  return std::is_sorted(v_.begin(), v_.end(), lt );
}

// ======================================================================

template< class Value >
bool
  cet::map_vector<Value>::
  lt ( value_type const & v1, value_type const & v2 )
{
  return v1.first < v2.first;
}

// ======================================================================

#endif
