#ifndef CETLIB__EXCEPTION_H
#define CETLIB__EXCEPTION_H

// ======================================================================
//
//  exception:  an exception type with category capability
//
// ----------------------------------------------------------------------
//
// Each exception is identified by a category string.  This string is a
// short word or phrase (no whitespace) describing the problem that was
// encountered.  These category identifiers can be concatenated when
// exceptions are caught and rethrown; the entire list can be accessed
// using the history() call.  Example:
//
//   try {
//     func();
//   }
//   catch (cet::exception & e)
//   {
//     throw cet::exception("DataCorrupt","encountered while unpacking",e);
//   }
//
// Information can be appended to the exception via operator <<.  Example:
//
//   if( (rc=func()) < 0 ) {
//     throw cet::exception("DataCorrupt")
//       << "I died with rc = " << rc
//       << std::endl;
//   }
//
// ----------------------------------------------------------------------
//
// Derived types are expected to adjust the category, either by
//   1) passing a string literal to the base class constructor, or
//   2) ensuring the developer gives a category name.
// Example:
// class infinite_loop
//   : public exception
// {
//   infinite_loop( std::string const & mesg )
//     : exception( "infinite_loop", mesg )
//   { }
// };
//
// ----------------------------------------------------------------------
//
// The output from what() has a simple format that makes it easy to
// locate the reason for and context of the error and separate it from
// the user-supplied free-format text.  The output from what() contains
// all the category and context information in a nested hierarchical format.
//
// ======================================================================


#include "cpp0x/type_traits"
#include <exception>
#include <iosfwd>
#include <list>
#include <sstream>
#include <string>


// ======================================================================


namespace cet {

  #if NO_LONGER_USED
  namespace detail {

    template< class D, class T
            , bool = std::is_base_of<cet::exception,D>::value
            >
      struct enable_if_an_exception
    { typedef  T  type };

    template< class D, class T >
      struct enable_if_an_exception<D,T,false>
    { };

  }  // namespace detail
  #endif  // NO_LONGER_USED


  class exception
    : public std::exception
  {
  public:
    typedef  std::string          Category;
    typedef  std::list<Category>  CategoryList;

    // --- c'tors, d'tors:
    explicit
      exception( Category const & category );
    exception( Category const & category
             , std::string const & message );
    exception( Category const & category
             , std::string const & message
             , exception const & another );
    exception( exception const & other );

    virtual
      ~exception( ) throw();

    // --- inspectors:

    virtual  char const *  what( ) const throw();
    virtual  std::string   explain_self( ) const;
    std::string            category( ) const;
    CategoryList const &   history( ) const;
    std::string            root_cause( ) const;

    // --- mutators:

    void  append( exception const & another );
    void  append( std::string const & more_information );
    void  append( char const more_information[] );

    // --- output:

    exception &  operator << ( char const stuff[] );
    template< typename T >
      exception &  operator << ( T const & stuff )
    { ost_ << stuff; return *this; }
    exception &  operator << ( std::ostream& f(std::ostream&) );
    exception &  operator << ( std::ios_base& f(std::ios_base& ) );

  private:
    std::ostringstream   ost_;
    CategoryList         category_;

    virtual  std::exception *  clone() const;
    virtual  void  rethrow();

  };  // exception


  std::ostream &
    operator << ( std::ostream & os, exception const & e );

}  // namespace cet


// ======================================================================


#endif  // CETLIB__EXCEPTION_H
