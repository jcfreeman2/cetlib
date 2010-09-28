// ======================================================================
//
// search_path
//
// ======================================================================


#include "cetlib/search_path.h"

#include "cetlib/exception.h"
#include <algorithm>   // transform
#include <cstdlib>     // getenv
#include <iterator>    // back_inserter
#include <sys/stat.h>  // stat

using cet::search_path;


// ======================================================================


static
std::string
  resolve_env_variable( std::string s )
{
  if( s[0] != '$' ) return s;

  std::string::size_type k = 1
                       , n = s.size() - 1;
  if     ( s[1] == '{'  && s[n] == '}' ) ++k, n -= 2;
  else if( s[1] == '('  && s[n] == ')' ) ++k, n -= 2;

  char const * p = std::getenv( s.substr(k,n).c_str() );
  if( p == 0 )
    throw cet::exception("SEARCH_PATH")
      << "Environment variable \"" << s
      << "\" was requested but not found.";

  return p;
}


static
std::string
  terminate_with_slash( std::string const & s )
{
  std::string::size_type sz = s.size();

  return s[sz-1] == '/' ?  s
                        :  s + '/';
}


static
bool
  file_exists( std::string const & filename )
{
  struct stat file_info;
  return 0 == stat( filename.c_str(), & file_info );
}


// ======================================================================


search_path::search_path( std::string const & path )
: dirs( )
{
  split( path, ':', std::back_inserter(dirs) );
  std::transform( dirs.begin(), dirs.end()
                , dirs.begin()
                , resolve_env_variable
                );
  std::transform( dirs.begin(), dirs.end()
                , dirs.begin()
                , terminate_with_slash
                );
}


std::string
  search_path::find_file( std::string filename )
{
  while( filename[0] == '/' )
    filename.erase(0, 1);

  for( std::vector<std::string>::const_iterator b = dirs.begin()
                                              , e = dirs.end()
     ; b != e; ++b ) {
    std::string fullpath = *b + filename;
    if( file_exists(fullpath) )
      return fullpath;
  }

  return "";
}



// ======================================================================
