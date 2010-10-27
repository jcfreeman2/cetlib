#ifndef CETLIB__CODED_EXCEPTION_H
#define CETLIB__CODED_EXCEPTION_H

// ======================================================================
//
//  coded_exception:  a family of enumeration-based exception types
//
// ======================================================================


#include "cetlib/exception.h"
#include <string>


namespace cet {
  template< typename Code
          , std::string translate( Code )
          >
    class coded_exception;
}

//#define EDM_MAP_ENTRY(map, ns, name) map[ns::name]=#name
//#define EDM_MAP_ENTRY_NONS(map, name) map[name]=#name

// ======================================================================

template< typename Code
        , std::string translate( Code )
        >
  class cet::coded_exception
  : public cet::exception
{
public:
  // --- c'tors, d'tor:

  explicit
    coded_exception( Code c )
  : exception( codeToString(c) )
  , category_( c )
  { }

    coded_exception( Code c, std::string const & m )
  : exception( codeToString(c), m )
  , category_( c )
  { }

    coded_exception( Code c, std::string const & m, exception const & e )
  : exception( codeToString(c), m, e )
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

  static  std::string
    codeToString( Code code )
  { return translate(code); }

private:
  Code category_;

};  // coded_exception<>

#endif  // CETLIB__CODED_EXCEPTION_H
