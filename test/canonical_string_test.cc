#include "cetlib/canonical_string.h"
#include <cstdlib>
#include <iostream>
#include <string>


void
  ensure( int which, bool claim )
{
  if( not claim )
    std::exit(which);
}

bool
  becomes( std::string const & input
         , std::string const & wanted
         )
{
  std::string result;
  if( ! cet::canonical_string(input, result) )
    return false;

  if( result == wanted )
    return true;

  std::cerr << "Input:  " << input  << '\n'
            << "Wanted: " << wanted << '\n'
            << "Result: " << result << '\n'
            ;
  return false;
}


std::string
  dquoted( std::string const & s )
{
  return '"' + s + '"';
}


std::string
  squoted( std::string const & s )
{
  return '\'' + s + '\'';
}


int
  main( )
{
  ensure( 1, becomes( "a"         , dquoted("a") ) );
  ensure( 2, becomes( squoted("a"), dquoted("a") ) );
  ensure( 3, becomes( dquoted("a"), dquoted("a") ) );
  ensure( 4, ! becomes( ""        ,  dquoted("") ) );

  ensure( 11, becomes( squoted("\n"), dquoted("\\n") ) );
  ensure( 12, becomes( dquoted("\n"), dquoted("\\n") ) );
  ensure( 13, becomes( squoted("\\"), dquoted("\\\\") ) );
  ensure( 14, becomes( dquoted("\\"), dquoted("\\\\") ) );
  ensure( 15, becomes( squoted("\\\\"), dquoted("\\\\\\\\") ) );
  ensure( 16, becomes( dquoted("\\\\"), dquoted("\\\\") ) );

  ensure( 21, becomes( squoted("\t\'\""), dquoted("\\t\\\'\\\"") ) );
  ensure( 22, becomes( dquoted("\t\'\""), dquoted("\\t\\\'\\\"") ) );

  return 0;

}  // main()
