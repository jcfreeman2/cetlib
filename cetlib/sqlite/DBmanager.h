#ifndef cetlib_sqlite_DBmanager_h
#define cetlib_sqlite_DBmanager_h

#include "cetlib/sqlite/helpers.h"

namespace sqlite {

  class DBmanager {
  public:

    DBmanager(std::string const& filename)
      : db_{openDatabaseFile(filename)}
    {}

    ~DBmanager() noexcept
    {
      sqlite3_close(db_);
    }

    sqlite3* get() const { return db_; }
    operator sqlite3*() { return db_; } // Maybe git rid of?

  private:
    sqlite3* db_;
  };

} //namespace sqlite

#endif /* cetlib_Ntuple_sqlite_DBmanager_h */

// Local Variables:
// mode: c++
// End:
