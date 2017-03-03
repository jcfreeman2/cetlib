#ifndef cetlib_sqlite_Connection_h
#define cetlib_sqlite_Connection_h

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
      Connection operator=(Connection&&) = delete;

    private:
      sqlite3* db_;
    };

  } //namespace sqlite
} // cet

#endif /* cetlib_sqlite_Connection_h */

// Local Variables:
// mode: c++
// End:
