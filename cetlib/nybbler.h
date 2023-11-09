#ifndef cetlib_nybbler_h
#define cetlib_nybbler_h

// ======================================================================
//
// nybbler: Half-byte manipulations
//
// ======================================================================

#include <string>

namespace cet {
  class nybbler;
}

// ======================================================================

class cet::nybbler {
public:
  nybbler() = default;
  explicit nybbler(std::string const& s) : s_(s) {}

  std::string as_hex() const;
  std::string as_char() const;

  nybbler& operator<<(std::string const&);

private:
  using uchar = unsigned char;

  std::string s_{};

  static uchar msn(char ch); // most significant nybble
  static uchar lsn(char ch); // least significant nybble

  static char to_hex(uchar nyb);
  static uchar to_nyb(char hex);

}; // nybbler

// ======================================================================

#endif /* cetlib_nybbler_h */

// Local variables:
// mode: c++
// End:
