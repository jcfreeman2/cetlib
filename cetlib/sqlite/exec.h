#ifndef cetlib_sqlite_exec_h
#define cetlib_sqlite_exec_h

#include "sqlite3.h"
#include <string>

namespace cet {
  namespace sqlite {
    void exec(sqlite3* db, std::string const& ddl);
  }
}

#endif /* cetlib_sqlite_exec_h */
