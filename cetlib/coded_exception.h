#ifndef CETLIB__CODED_EXCEPTION_H
#define CETLIB__CODED_EXCEPTION_H

// ======================================================================
//
//  coded_exception:  a family of enumeration-based exception types
//
// ======================================================================


#include "cetlib/exception.h"
#include <map>
#include <string>


namespace cet {
  template< typename Code
          , std::map<Code,std::string> const & f()
          >
    class coded_exception;
}

//#define EDM_MAP_ENTRY(map, ns, name) map[ns::name]=#name
//#define EDM_MAP_ENTRY_NONS(map, name) map[name]=#name

// ======================================================================

template< typename Code
        , std::map<Code,std::string> const & translate()
        >
  class cet::coded_exception
  : public cet::exception
{
public:
  // --- c'tors, d'tor:

  explicit
    coded_exception( Code c )
  : exception( to_string(c) )
  , category_( c )
  { }

    coded_exception( Code c, std::string const & m )
  : exception( to_string(c), m )
  , category_( c )
  { }

    coded_exception( Code c, std::string const & m, exception const & e )
  : exception( to_string(c), m, e )
  , category_( c )
  { }

  virtual
    ~coded_exception() throw()
  { }

  // --- inspectors:

  Code
    categoryCode() const
  { return category_; }

  int
    returnCode() const
  { return static_cast<int>(category_); }

private:
  Code category_;

  static std::string
    to_string( Code c )
  {
    typedef  typename std::map<Code,std::string>::const_iterator
             iterator;

    iterator  it = translate().find(c);
    return it == translate().end()  ?  std::string("Unknown code")
                                    :  it->second;
  }

};  // coded_exception<>

#endif  // CETLIB__CODED_EXCEPTION_H
