#ifndef CETLIB_INCLUDER_H
#define CETLIB_INCLUDER_H

// ======================================================================
//
// includer: A container-like data structure that
//           a) transparently handles #include'd files, and
//           b) can trace back its iterators
//
// ======================================================================

#include "cetlib/filepath_maker.h"
#include <string>
#include <vector>

namespace cet {
  class includer;
}

// ----------------------------------------------------------------------

class cet::includer
{
  typedef  std::string::size_type       uint;

public:
  typedef  std::string::const_iterator  const_iterator;

  explicit        includer( std::string const   & filename
                          , cet::filepath_maker & abs_filename
                          );
  const_iterator  begin   ( ) const  { return text.begin(); }
  const_iterator  end     ( ) const  { return text.end  (); }
  std::string     whereis ( const_iterator const & it ) const;

private:
  struct frame
  {
    uint         including_framenum;
    std::string  filename;
    uint         starting_linenum;
    uint         starting_textpos;

    frame( int framenum, std::string filename, int linenum, int textpos )
      : including_framenum( framenum )
      , filename          ( filename )
      , starting_linenum  ( linenum )
      , starting_textpos  ( textpos )
    { }
  };

  std::string         text;
  std::vector<frame>  frames;

  void  include ( int                   including_framenum
                , std::string const   & filename
                , cet::filepath_maker & abs_filename
                );
  void  debug   ( ) const;

};  // includer

// ======================================================================

#endif
