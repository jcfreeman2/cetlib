#ifndef CETLIB__SEARCH_PATH_H
#define CETLIB__SEARCH_PATH_H

// ======================================================================
//
// search_path: resolve any given relative filename per a given list of
//              absolute pathnames
//
// ======================================================================


#include "cetlib/split.h"
#include <string>
#include <vector>


namespace cet {
  class search_path;
}


// ======================================================================


class cet::search_path
{
public:
  typedef  std::string::size_type  size_type;

  search_path( std::string const & path );

  bool  empty( ) const  { return dirs.empty(); }
  size_type  size( ) const  { return dirs.size(); }
  std::string const &  operator [] ( int k ) const  { return dirs.at(k); }

  std::string  find_file( std::string filename );

private:
  std::vector<std::string>  dirs;

};  // search_path



#endif  // CETLIB__SEARCH_PATH_H
