// ======================================================================
//
// search_path: Seek filename or pattern in a given list of pathnames
//
// ======================================================================

#include "cetlib/search_path.h"

#include "cetlib/exception.h"
#include "cetlib/filesystem.h"
#include "cetlib/getenv.h"
#include <regex>
#include <dirent.h>
#include <iterator>
#include <sys/stat.h>

using cet::search_path;

std::string exception_category("search_path");

// ----------------------------------------------------------------------
// c'tor:

search_path::search_path( std::string const & arg )
: dirs( )
, end ( )
{
  if( ! arg.empty() )
    split( arg.find(':') == std::string::npos
           ?  cet::getenv(arg)  // arg is an env var
           :  arg               // arg is a path
         , ':'
         , std::back_inserter(dirs)
         );

  if( dirs.empty() )
    dirs.push_back( std::string() );

  end = dirs.end();
}  // c'tor

// ----------------------------------------------------------------------

std::string
  search_path::to_string( ) const
{
  std::string printable_path = dirs[0];
  for( std::size_t k = 1; k != dirs.size(); ++k )
    printable_path.append(1, ':')
                  .append(dirs[k]);
  return printable_path;
}

// ----------------------------------------------------------------------
// find_file() overloads:

std::string
  search_path::find_file( std::string const & filename ) const
{
  std::string result;
  if( find_file(filename, result) )
    return result;
  else
    throw cet::exception(exception_category)
      << "Can't find file \"" << filename << '\"';
}  // find_file()

bool
  search_path::find_file( std::string const & filename
                        , std::string       & result
                        ) const
{
  if( filename.empty() )
    return false;

  for( std::vector<std::string>::const_iterator it = dirs.begin()
     ; it != end; ++it ) {
    std::string fullpath = *it + '/' + filename;
    for( std::size_t k;  (k = fullpath.find("//")) != std::string::npos;  ) {
      fullpath.erase(k,1);
    }
    if( cet::file_exists(fullpath) )  {
      result = fullpath;
      return true;
    }
  }

  return false;
}  // find_file()

// ----------------------------------------------------------------------

std::size_t
  search_path::find_files( std::string const        & pat
                         , std::vector<std::string> & out
                         ) const
{
  std::regex re(pat);

  std::size_t     count = 0u;
  std::size_t     err = 0u;
  struct dirent   entry;
  struct dirent * result = 0;

  for( std::vector<std::string>::const_iterator it = dirs.begin()
     ; it != end; ++it ) {
    DIR * dd = opendir(it->c_str());
    if( dd == 0 )
      continue;
    while( ! (err = readdir_r(dd, &entry, &result))
           && result != 0 ) {
      if( std::regex_match(entry.d_name, re) )
        out.push_back(*it + '/' + entry.d_name), ++count;
    }
    closedir(dd);
    if( result != 0 )
      throw cet::exception(exception_category)
        << "Failed to read directory \"" << *it << "\"; error num = " << err;
  }  // for

  return count;
}  // find_files()

// ======================================================================
