// ======================================================================
//
// exception
//
// ======================================================================


#include "cetlib/exception.h"
  using cet::exception;

#include <cctype>

typedef  exception::Category      Category;
typedef  exception::CategoryList  CategoryList;


// ======================================================================
// helpers:

bool
  ends_with_whitespace( std::string const & s )
{
  return s.empty() || std::isspace( *(s.end()-1) );
}

bool
  ends_with_newline( std::string const & s )
{
  return s.empty() || *(s.end()-1) == '\n';
}


std::exception *
  exception::clone() const
{
  return new exception(*this);
}


void
  exception::rethrow()
{
  throw *this;
}


// ======================================================================
// c'tors, d'tors:

exception::exception( Category const & category )
  : std::exception( )
  , ost_          ( )
  , category_     ( 1, category )
{ }


exception::exception( Category    const & category
                    , std::string const & message )
  : std::exception( )
  , ost_          ( )
  , category_     ( 1, category )
{
  ost_ << message;
  if( ! ends_with_whitespace(message) )
    ost_ << ' ';
}


exception::exception( Category    const & category
                    , std::string const & message
                    , exception   const & another )
  : std::exception( )
  , ost_          ( )
  , category_     ( 1, category )
{
  ost_ << message;
  if( ! ends_with_whitespace(message) )
    ost_ << ' ';
  category_.push_back( another.category() );
  append( another );
}


exception::~exception( ) throw()
{ }


// ======================================================================
// copy c'tor:

exception::exception( exception const & other )
  : std::exception( )
  , ost_          ( )
  , category_     ( other.category_ )
{
  ost_ << other.ost_.str();
}


// ======================================================================
// inspectors:

char const *
  exception::what( ) const throw()
{
  return explain_self().c_str();
}


std::string
  exception::explain_self( ) const
{
  std::ostringstream ost;

  ost << "---- " << category() << " BEGIN\n";

  std::string part( ost_.str() );
  ost << part;
  if( ! ends_with_newline(part) )
    ost << "\n";

  ost << "---- " << category() << " END\n";

  return ost.str();
}


std::string
  exception::category( ) const
{
  return category_.front();
}


CategoryList const &
  exception::history( ) const
{
  return category_;
}


std::string
  exception::root_cause( ) const
{
  return category_.back();
}


// ======================================================================
// mutators:

void
  exception::append( exception const & another )
{
  ost_ << another.explain_self();
}


void
  exception::append( std::string const & more_information )
{
  ost_ << more_information;
}


void
  exception::append( char const more_information[] )
{
  ost_ << more_information;
}


// ======================================================================
// output:

exception &
  exception::operator << ( char const stuff[] )
{
  ost_ << stuff;
  return *this;
}


exception &
  exception::operator << ( std::ostream & f(std::ostream & ) )
{
  f(ost_);
  return *this;
}


exception &
  exception::operator << ( std::ios_base & f(std::ios_base & ) )
{
  f(ost_);
  return *this;
}


std::ostream &
  cet::operator << ( std::ostream & os, exception const & e )
{
  return os << e.explain_self();
}


// ======================================================================
