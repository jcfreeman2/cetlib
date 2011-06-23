// ======================================================================
//
// canonical_string: Transform a string into a canonical form
//
// ======================================================================

#include "cetlib/canonical_string.h"

#include "cetlib/exception.h"
#include <cctype>

using std::string;

// ----------------------------------------------------------------------

string
  cet::escape( string const & str )
{
  string result;
  for( string::const_iterator it = str.begin()
                            , e  = str.end(); it != e; ++it ) {
    switch( *it )  {
      case '\"':  result.append("\\\"");  break;
      case '\'':  result.append("\\\'");  break;
      case '\\':  result.append("\\\\");  break;
      case '\n':  result.append("\\n" );  break;
      case '\t':  result.append("\\t" );  break;
      default  :  if( std::isprint(*it) ) result.append(1, *it);
                  else  throw cet::exception("unprintable character");
    }
  }
  return result;
}  // escape()

// ----------------------------------------------------------------------

string
  cet::unescape( string const & str )
{
  string result;
  for( string::const_iterator it = str.begin()
                            , e  = str.end(); it != e; ++it ) {
    char ch = *it;
    if( ch == '\\' && it != e-1 ) {
      switch( *++it ) {
        case '\"':  ch = '\"';  break;
        case '\'':  ch = '\'';  break;
        case '\\':  ch = '\\';  break;
        case 'n' :  ch = '\n';  break;
        case 't' :  ch = '\t';  break;
        default  :  throw cet::exception("unknown escape: \\") << *it;
      }
    }
    result.append(1, ch);
  }
  return result;
}  // unescape()

// ----------------------------------------------------------------------

bool
  cet::canonical_string( string const & s
                       , string       & result
                       )
try
{
  if( s.empty() )
    return false;

  string value;
  if( s[0] == '"' && s.end()[-1] == '"' )
    value = cet::unescape( s.substr(1, s.size()-2) );
  else if( s[0] == '\'' && s.end()[-1] == '\'' )
    value = s.substr(1, s.size()-2);
  else
    value = s;

  result.append( 1, '"' )
        .append( cet::escape(value) )
        .append( 1, '"' )
        ;
  return true;
}
catch( ... ) {
  return false;
}  // canonical_string()

// ======================================================================
