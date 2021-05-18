#include "cetlib/sqlite/Connection.h"
// vim: set sw=2 expandtab :

#include "sqlite3.h"

using namespace cet::sqlite;

Connection::~Connection() noexcept
{
  // It is safe to call sqlite3_close on a null db_.
  sqlite3_close(db_);
}
