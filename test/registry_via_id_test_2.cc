// ======================================================================
//
// test registry_via_id's requirement that the value type have
// a member function that is:
//   1) named id, and
//   2) returns a value of the key type
//
// ======================================================================

#include "cetlib/registry_via_id.h"

#define SUCCEED

struct val
{
#ifdef SUCCEED
  int
#else  // FAIL
  char *
#endif  // SUCCEED or FAIL
    id() const { return 0; }
};

typedef  cet::registry_via_id<int,val>  reg;

int
  main( )
{
  reg::put( val() );

  return 0;
}  // main()
