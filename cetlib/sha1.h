#ifndef CETLIB_SHA1_H
#define CETLIB_SHA1_H

// ======================================================================
//
// sha1: Secure Hash Algorithm as defined in FIPS PUB 180-1, 1993-05-11
//
// ======================================================================


#include "boost/array.hpp"
#include "cetlib/polarssl_sha1.h"
#include <cstring>
#include <string>


namespace cet {
  class sha1;
}


// ======================================================================


class cet::sha1
{
public:
  typedef  unsigned char           uchar;
  typedef  boost::array<uchar,20>  digest_t;

  sha1() { reset(); }
  sha1( std::string const & mesg ) { reset(); operator<<(mesg); }

  void  reset( ) { sha1_starts( & context ); }

  sha1 &  operator << ( std::string const & mesg )
  {
    sha1_update( & context
               , (uchar const *)( & mesg[0] )
               , mesg.size()
               );
  }

  digest_t  digest( )
  {
    digest_t result;
    sha1_finish( & context, & result[0] );
    std::memset( & context, 0, sizeof(sha1_context) );
    return result;
  }

private:
  sha1_context  context;

};  // sha1


// ======================================================================


#endif  // CETLIB_SHA1_H
