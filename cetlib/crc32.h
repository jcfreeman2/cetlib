#ifndef CETLIB_CRC32_H
#define CETLIB_CRC32_H

// ======================================================================
//
// crc32: Calculate a CRC32 checksum
//
// ======================================================================

#include "cpp0x/cstdint"
#include <string>

namespace cet {
  class crc32;
}

// ======================================================================

class cet::crc32
{
public:
  typedef  unsigned char  uchar;
  typedef  std::uint32_t  digest_t;

  crc32( ) { reset(); }
  explicit
    crc32( std::string const & mesg ) { reset(); operator<<(mesg); }
  explicit
    crc32( char const mesg ) { reset(); operator<<(mesg); }

  void  reset( );

  crc32 &
    operator << ( std::string const & mesg );
  crc32 &
    operator << ( char const mesg );

  digest_t
    digest( );

private:
  digest_t  context;

};  // crc32

// ======================================================================

#endif
