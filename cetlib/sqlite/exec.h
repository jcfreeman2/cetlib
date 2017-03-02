#include "sqlite3.h"
#include <string>

namespace sqlite {
  void exec(sqlite3* db,std::string const& ddl);
}
