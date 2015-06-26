// ======================================================================
//
// exception
//
// ======================================================================

#include "cetlib/exception.h"
#include <cctype>

using cet::exception;

typedef  exception::Category      Category;
typedef  exception::CategoryList  CategoryList;

// ======================================================================
// helpers:

static  bool
  ends_with_whitespace( std::string const & s )
{
  return s.empty() || std::isspace( *(s.end()-1) );
}

static  bool
  ends_with_newline( std::string const & s )
{
  return s.empty() || *(s.end()-1) == '\n';
}

static  std::string
indent_string( std::string const &s )
{
   static char const *indent = "  ";
   std::string result(indent);
   result.append(s);
   size_t cpos = 0;
   while ((cpos = result.find('\n', cpos)) != std::string::npos) {
      ++cpos;
      if (cpos != result.size()) {
         result.insert(cpos, indent);
      }
   }
   return result;
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

exception::exception( Category const & c )
  : std::exception( )
  , ost_          ( )
  , category_     ( { c } )
  , what_         ( )
{ }

exception::exception( Category    const & c
                    , std::string const & m )
  : std::exception( )
  , ost_          ( )
  , category_     ( { c } )
  , what_         ( )
{
  ost_ << m;
  if( ! ends_with_whitespace(m) )
    ost_ << ' ';
}

exception::exception( Category    const & c
                    , std::string const & m
                    , exception   const & e )
  : std::exception( )
  , ost_          ( )
  , category_     ( { c } )
  , what_         ( )
{
  if ( ! m.empty() ) ost_ << m << '\n';
  category_.insert(category_.end(), e.history().begin(), e.history().end());
  append( e );
}

exception::~exception( ) noexcept
{ }

// ======================================================================
// copy c'tor:

exception::exception( exception const & other )
  : std::exception( )
  , ost_          ( )
  , category_     ( other.category_ )
  , what_         ( other.what_ )
{
  ost_ << other.ost_.str();
}

// ======================================================================
// inspectors:

char const *
  exception::what( ) const throw()
{
  what_ = explain_self();
  return what_.c_str();
}

std::string
  exception::explain_self( ) const
{
  std::ostringstream ost;

  ost << "---- " << category() << " BEGIN\n";

  std::string part( indent_string(ost_.str()) );
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
  exception::append( exception const & e ) const
{
  ost_ << e.explain_self();
}

void
  exception::append( std::string const & more_information ) const
{
  ost_ << more_information;
}

void
  exception::append( char const more_information[] ) const
{
  ost_ << more_information;
}

void
  exception::append( std::ostream& f(std::ostream&) ) const
{
  f(ost_);
}

void
  exception::append( std::ios_base& f(std::ios_base&) ) const
{
  f(ost_);
}

// ======================================================================

std::ostream &
  cet::operator << ( std::ostream & os, exception const & e )
{
  return os << e.explain_self();
}

// ======================================================================
