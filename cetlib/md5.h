#ifndef CETLIB_MD5_H
#define CETLIB_MD5_H

// ======================================================================
//
// md5: Secure Hash Algorithm as defined in RFC 1321, 1992-04
//
// ======================================================================

#include "cpp0x/array"
#include "polarssl/md5.h"
#include <cstring>
#include <string>

namespace cet {
  class md5;
}

// ======================================================================

class cet::md5
{
public:
  static  std::size_t const             digest_sz  = 16;
  typedef  unsigned char                uchar;
  typedef  std::array<uchar,digest_sz>  digest_t;

  md5( ) { reset(); }
  explicit
    md5( std::string const & mesg ) { reset(); operator<<(mesg); }
  explicit
    md5( char const mesg ) { reset(); operator<<(mesg); }

  void
    reset( ) { polarssl::md5_starts( & context ); }

  md5 &
    operator << ( std::string const & mesg )
  {
    polarssl::md5_update( & context
                         , (uchar const *)( & mesg[0] )
                         , mesg.size()
                         );
    return *this;
  }

  md5 &
    operator << ( char const mesg )
  {
    polarssl::md5_update( & context
                         , (uchar const *)( & mesg )
                         , 1u
                         );
    return *this;
  }

  digest_t
    digest( )
  {
    digest_t result;
    polarssl::md5_finish( & context, & result[0] );
    std::memset( & context, 0, sizeof(polarssl::md5_context) );
    return result;
  }

private:
  polarssl::md5_context  context;

};  // md5

// ======================================================================

#endif
