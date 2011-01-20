// ======================================================================
//
// canonical_number - transform a number string into a canonical form
//
// "Number string" is defined by the following regular expression:
//      ^[-+]?([0-9]*\.?[0-9]+|[0-9]+\.?[0-9]*)([eE][-+]?[0-9]+)?$
//
// ======================================================================

#include "cetlib/canonical_number.h"

#include "boost/lexical_cast.hpp"
#include <cctype>
#include <cstddef>
#include <cstdlib>

using boost::lexical_cast;
using std::abs;


bool
  cet::canonical_number( std::string const & value
                       , std::string       & result
                       )
try
{
  std::string::const_iterator       it  = value.begin();
  std::string::const_iterator const end = value.end();

  // extract sign, if any
  std::string sign( *it == '-' ?  "-" : "" );
  if( *it == '+'  || *it == '-' )
    ++it;

  // extract optional whole part
  std::string whole;
  for( ; it != end  &&  std::isdigit(*it); ++it )
    whole.append( 1, *it );

  // extract fraction part
  std::string fraction;
  if( it != end  &&  *it == '.' ) {
    while( ++it != end  &&  std::isdigit(*it) )
      fraction.append( 1, *it );
  }

  // exponent part
  std::string exponent;
  if( it != end  &&  (*it == 'E' || *it == 'e') ) {
    if( ++it == end )
      return false;
    if( *it == '+' || *it == '-') {
      exponent = *it;
      if( ++it == end )
        return false;
    }
    for( ; it != end  &&  std::isdigit(*it); ++it )
      exponent.append( 1, *it );
  }

  // consumed everything?
  if( it != end )
    return false;

  // require at least one digit
  std::string digits = whole + fraction;
  if( digits.empty() )
    return false;
  std::size_t ndig = digits.size();

  // calculate exponent as if decimal point were at left
  long exp = std::atoi(exponent.c_str()) + whole.size();

  // discard trailing zeroes
  while( ndig > 1  &&  digits[ndig-1] == '0' )
    digits.erase( --ndig, 1 );
  // discard leading zeroes
  for( ; ndig > 1  &&  digits[0] == '0'; --exp, --ndig )
    digits.erase( 0, 1 );

  // produce result
  if( digits == "0" ) {
    result.append( digits );
    return true;
  }
  result.append(sign);
  if( long(ndig) <= exp  &&  exp <= 6L ) { // non-neg exp < e6?
    result.append( digits )
          .append( exp-ndig, '0' );
  }
  else {
    if( ndig > 1 )
      digits.insert(digits.begin()+1, '.');
    --exp;
    result.append(digits);
    if( exp != 0 ) {
      result.append( 1, 'e' )
            .append( lexical_cast<std::string>(exp) );
    }
  }
  return true;
}
catch( ... )
{
  return false;
}  // canonical_number()

// ======================================================================
