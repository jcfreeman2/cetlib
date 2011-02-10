#ifndef CETLIB_FILEPATH_MAKER_H
#define CETLIB_FILEPATH_MAKER_H

// ======================================================================
//
// filepath_maker: a family of related policies governing the translation
//                 of a filename into a fully-qualified filepath
//
// ======================================================================

#include "cetlib/search_path.h"
#include <string>

namespace cet {
  class filepath_maker;
  class filepath_lookup;
  class filepath_lookup_nonabsolute;
  class filepath_lookup_after1;
}

// ----------------------------------------------------------------------

class cet::filepath_maker
{
public:
  filepath_maker( )
  { }

  virtual std::string
    operator () ( std::string const & filename );

  virtual ~filepath_maker( );

};  // filepath_maker

// ----------------------------------------------------------------------

class cet::filepath_lookup
  : public cet::filepath_maker
{
public:
  filepath_lookup( std::string paths );

  virtual std::string
    operator () ( std::string const & filename );

  virtual ~filepath_lookup( );

private:
  cet::search_path paths;

};  // filepath_lookup

// ----------------------------------------------------------------------

class cet::filepath_lookup_nonabsolute
  : public cet::filepath_maker
{
public:
  filepath_lookup_nonabsolute( std::string paths );

  virtual std::string
    operator () ( std::string const & filename );

  virtual ~filepath_lookup_nonabsolute( );

private:
  cet::search_path paths;

};  // filepath_lookup_nonabsolute

// ----------------------------------------------------------------------

class cet::filepath_lookup_after1
  : public cet::filepath_maker
{
public:
  filepath_lookup_after1( std::string paths );

  virtual std::string
    operator () ( std::string const & filename );

  void
    reset( );

  virtual ~filepath_lookup_after1( );

private:
  bool             after1;
  cet::search_path paths;

};  // filepath_lookup_after1

// ======================================================================

#endif
