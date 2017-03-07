#ifndef cetlib_sqlite_detail_column_constraint_h
#define cetlib_sqlite_detail_column_constraint_h

#include <ostream>
#include <string>

namespace cet {
  namespace sqlite {

    struct primary_key {
      static std::string snippet() { return " PRIMARY KEY"; }
    };

  }
}

#endif
