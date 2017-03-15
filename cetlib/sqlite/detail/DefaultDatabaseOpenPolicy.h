#ifndef cetlib_sqlite_detail_DefaultDatabaseOpenPolicy_h
#define cetlib_sqlite_detail_DefaultDatabaseOpenPolicy_h

#include "cetlib/sqlite/helpers.h"

#include <string>

namespace cet {
  namespace sqlite {
    namespace detail {
      class DefaultDatabaseOpenPolicy {
      public:
        sqlite3* open(std::string const& file_name)
        {
          return openDatabaseConnection(file_name);
        }
      };
    }
  }
}

#endif

// Local variables:
// mode: c++
// End:
