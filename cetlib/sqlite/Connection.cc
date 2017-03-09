#include "cetlib/sqlite/Connection.h"
#include "cetlib/sqlite/helpers.h"

using namespace cet::sqlite;

Connection::Connection(std::string const& filename)
  : db_{openDatabaseConnection(filename)}
{}

Connection::~Connection() noexcept
{
  // It is safe to call sqlite3_close on a null db_.
  sqlite3_close(db_);
}
