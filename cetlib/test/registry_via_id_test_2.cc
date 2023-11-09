// ======================================================================
//
// test registry_via_id's requirement that the value type have
// a member function that is:
//   1) named id, and
//   2) returns a value of the key type
//
// ======================================================================

#include "cetlib/registry_via_id.h"

using success_t = int;
using fail_t = char*; // anything other than success_t

struct val {
  success_t
  // fail_t
  id() const
  {
    return 0;
  }
};

using reg = cet::registry_via_id<success_t, val>;

int
main()
{
  reg::put(val());

  return 0;
} // main()
