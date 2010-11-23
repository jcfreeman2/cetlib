// ======================================================================
//
// canonical_number - transform a number string into a canonical form
//
// "Number string" is defined by the following regular expression:
//   ^[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?$
//
// ======================================================================

#include "cetlib/canonical_number.h"

#include "boost/lexical_cast.hpp"
#include <cctype>   // isdigit
#include <cstddef>  // size_t
#include <cstdlib>  // abs, atoi

using namespace boost;
using namespace std;


bool
  cet::canonical_number( std::string const & value
                       , std::string       & result
                       )
{
  std::string::const_iterator       it = value.begin();
  std::string::const_iterator const e  = value.end();

  // optional sign
  std::string sign( *it == '-' ?  "-" : "" );
  if( *it == '+'  || *it == '-' )
    ++it;

  // whole part
  std::string whole;
  for( ; it != e  &&  std::isdigit(*it); ++it )
    whole.append( 1, *it );

  // fraction part
  std::string fraction;
  if( it != e  &&  *it == '.' ) {
    while( ++it != e  &&  std::isdigit(*it) )
      fraction.append( 1, *it );
  }

  // exponent part
  std::string exponent;
  if( it != e  &&  (*it == 'E' || *it == 'e') ) {
    if( ++it == e )
      return false;
    if( *it == '+' || *it == '-') {
      exponent = *it;
      if( ++it == e )
        return false;
    }
    for( ; it != e  &&  std::isdigit(*it); ++it )
      exponent.append( 1, *it );
  }

  // consumed everything?
  if( it != e )
    return false;

  // require at least one digit
  std::string digits = whole + fraction;
  if( digits.empty() )
    return false;
  std::size_t ndig = digits.size();

  // calculate exponent as if radix point were at left
  long exp = std::atoi(exponent.c_str()) + whole.size();

  // discard trailing zeroes
  while( ndig > 1  &&  digits[ndig-1] == '0' )
    digits.erase( --ndig, 1 );
  // discard leading zeroes
  for( ; ndig > 1  &&  digits[0] == '0'; --exp, --ndig )
    digits.erase( 0, 1 );

  // produce result
  result.append(sign);
  if( ndig <= 6  &&  ndig <= exp  &&  exp <= 6 ) { // < 1e6?
    result.append( digits )
          .append( exp-ndig, '0' );
  }
  else {
    digits.insert(digits.begin()+1, '.'); --exp;
    result.append(digits);
    if( exp != 0 ) {
      result.append( 1, 'e' )
            .append( 1, exp < 0 ? '-' : '+' )
            .append( lexical_cast<std::string>(std::abs(exp)) );
    }
  }

  return true;

}  // canonical_number()

// ======================================================================
