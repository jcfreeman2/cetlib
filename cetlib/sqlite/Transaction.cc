#include "cetlib/sqlite/Exception.h"
#include "cetlib/sqlite/Transaction.h"

#include <cassert>
#include "sqlite3.h"

cet::sqlite::Transaction::Transaction(sqlite3* db) :
  db_{db}
{
  assert(db_);
  int const rc {sqlite3_exec(db_, "BEGIN;", nullptr, nullptr, nullptr)};
  if (rc != SQLITE_OK) {
    throw sqlite::Exception{sqlite::errors::SQLExecutionError,"Failed to start SQLite transaction"};
  }
}

cet::sqlite::Transaction::~Transaction() noexcept
{
  // We can't throw an exception from our destructor, so we just
  // swallow any error.
  if (db_) {
    sqlite3_exec(db_, "ROLLBACK;", nullptr, nullptr, nullptr);
  }
}

void
cet::sqlite::Transaction::commit()
{
  assert(db_);
  int const rc {sqlite3_exec(db_, "COMMIT;", nullptr, nullptr, nullptr)};
  if (rc != SQLITE_OK) {
    throw sqlite::Exception{sqlite::errors::SQLExecutionError,"Failed to commit SQLite transaction"};
  }
  db_ = nullptr;
}
