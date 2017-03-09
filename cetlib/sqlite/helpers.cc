#include <cassert>
#include <cmath>
#include <regex>

#include "cetlib/sqlite/Exception.h"
#include "cetlib/sqlite/detail/normalize_statement.h"
#include "cetlib/sqlite/helpers.h"

//=================================================================
// hasTableWithSchema(db, name, cnames) returns true if the db has a
// table named 'name', with columns named 'cns' suitable for carrying
// a tuple<ARGS...>. It returns false if there is no table of that
// name, and throws an exception if there is a table of the given name
// but it does not match both the given column names and column types.
bool
cet::sqlite::hasTableWithSchema(sqlite3* db, std::string const& name, std::string expectedSchema)
{
  query_result<std::string> res;
  res << select("sql").from(db, "sqlite_master").where("type=\"table\" and name=\""s+name+'"');

  if (res.empty()) {
    return false;
  }

  // This is a somewhat fragile way of validating schemas.  A better
  // way would be to rely on sqlite3's insertion facilities to
  // determine if an insert of in-memory data would be compatible with
  // the on-disk schema.  This would require creating a temporary
  // table (so as to avoid inserting then deleting a dummy row into
  // the desired table)according to the on-disk schema, and inserting
  // some default values according to the requested schema.
  std::string retrievedSchema {unique_value(res)};
  detail::normalize_statement(retrievedSchema);
  detail::normalize_statement(expectedSchema);
  if (retrievedSchema == expectedSchema) {
    return true;
  }

  throw sqlite::Exception(sqlite::errors::SQLExecutionError)
    << "Existing database table name does not match description:\n"
    << "   DDL on disk: " << retrievedSchema << '\n'
    << "   Current DDL: " << expectedSchema << '\n';
}

namespace cet {
  namespace sqlite {
    namespace detail {

      //================================================================
      // The locking mechanisms for nfs systems are deficient and can
      // thus wreak havoc with sqlite, which depends upon them.  In
      // order to support an sqlite database on nfs, we use a URI,
      // explicitly including the query parameter: 'nolock=1'.  We will
      // have to revisit this choice once we consider multiple
      // processes/threads writing to the same database file.
      inline std::string assembleURI(std::string const& filename)
      {
        // Arbitrary decision: don't allow users to specify a URI since
        // they may (unintentionally) remove the 'nolock' parameter,
        // thus potentially causing issues with nfs.
        if (filename.substr(0,5) == "file:") {
          throw sqlite::Exception{sqlite::errors::OtherError}
          << "art does not allow an SQLite database filename that starts with 'file:'.\n"
               << "Please contact artists@fnal.gov if you believe this is an error.";
        }
        return "file:"+filename+"?nolock=1";
      }

    } // detail
  } // sqlite
} // cet

sqlite3*
cet::sqlite::openDatabaseConnection(std::string const& filename)
{
  sqlite3* db {nullptr};
  auto const& uri = detail::assembleURI(filename);
  int const rc {sqlite3_open_v2(uri.c_str(),
                                &db,
                                SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE|SQLITE_OPEN_URI,
                                nullptr)};
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    throw sqlite::Exception{sqlite::errors::SQLExecutionError}
    << "Failed to open SQLite database\n"
         << "Return code: " << rc;
  }

  assert(db);
  return db;
}

//=======================================================================
void
cet::sqlite::delete_from(sqlite3* const db, std::string const& tablename)
{
  exec(db, "delete from "s+tablename+';');
}

void
cet::sqlite::drop_table(sqlite3* const db, std::string const& tablename)
{
  exec(db, "drop table "s+tablename+';');
}

void
cet::sqlite::drop_table_if_exists(sqlite3* const db, std::string const& tablename)
{
  exec(db, "drop table if exists "s+tablename+';');
}

unsigned
cet::sqlite::nrows(sqlite3* db, std::string const& tablename)
{
  query_result<unsigned> r;
  r << select("count(*)").from(db, tablename);
  return unique_value(r);
}
