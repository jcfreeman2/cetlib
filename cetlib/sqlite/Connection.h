#ifndef cetlib_sqlite_Connection_h
#define cetlib_sqlite_Connection_h

//====================================================================
// A Connection is an RAII-motivated class that automatically handles
// opening and closing of the database handle.
//
// Note that whenever a Connection object is created, database locking
// is automatically disabled to accommodate operations on NFS systems.
// To that end, care must be taken to ensure that updates to the same
// database via Connection objects must be serialized.
//====================================================================

#include <string>
#include "sqlite3.h"

namespace cet {
  namespace sqlite {

    class Connection {
    public:

      explicit Connection(std::string const& filename);
      ~Connection() noexcept;

      sqlite3* get() const { return db_; }
      operator sqlite3*() { return db_; }

      // Non-copyable
      Connection(Connection const&) = delete;
      Connection& operator=(Connection const&) = delete;

    private:
      sqlite3* db_;
    };

  } //namespace sqlite
} // cet

#endif /* cetlib_sqlite_Connection_h */

// Local Variables:
// mode: c++
// End:
