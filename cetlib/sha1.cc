#include "cetlib/sha1.h"
#include <cstring>

using cet::sha1;

sha1::sha1() {
  reset();
}

sha1::sha1(std::string const& mesg)
{
  reset();
  operator<<(mesg);
}

sha1::sha1(char const mesg)
{
  reset();
  operator<<(mesg);
}

void
sha1::reset()
{
#ifdef __APPLE__
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
  SHA1_Init(&context);
}

sha1&
sha1::operator<<(std::string const& mesg)
{
  uchar const* data = reinterpret_cast<uchar const*>(&mesg[0]);
#ifdef __APPLE__
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
  SHA1_Update(&context, data, mesg.size());
  return *this;
}

sha1&
sha1::operator<<(char const mesg)
{
  uchar const* data = reinterpret_cast<uchar const*>(&mesg);
#ifdef __APPLE__
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
  SHA1_Update(&context, data, 1u);
  return *this;
}

sha1::digest_t
sha1::digest()
{
  digest_t result;
#ifdef __APPLE__
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
  SHA1_Final(&result[0], &context);
  std::memset(&context, 0, sizeof(SHA_CTX));
  return result;
}
