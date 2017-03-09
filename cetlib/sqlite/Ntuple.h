#ifndef cetlib_Ntuple_h
#define cetlib_Ntuple_h

// =====================================================================
//
// Ntuple
//
// The Ntuple class template is an interface for inserting into an
// SQLite database in a type-safe and thread-safe manner.  It is not
// intended for this facility to provide facilities for making SQLite
// database queries.  For querying, consider using the
// 'cet::sqlite::select' utilities.
//
// WARNING: At the moment, two Ntuple instances that access the same
//   database will NOT be thread-safe because locking is disabled to
//   accommodate NFS.  This will be adjusted in a future commit so
//   that access to a database is appropriately serialized across
//   Ntuple instances.
//
// Construction
// ------------
//
// There are two Ntuple c'tor signatures with opposite semantics:
//
//   (1) The c'tor that accepts a database handle object of type
//        (convertible to) sqlite3* implies a non-owning Ntuple object
//        that does not close the SQLite database whenever the Ntuple
//        d'tor is called.
//
//   (2) The c'tor that accepts a filename implies an owning object,
//       for which sqlite3_close IS called whenever the Ntuple object
//       is destroyed.
//
// In addition to the SQLite table name that the constructed Ntuple
// object will refer, a set of column names is also required in the
// form of an std::array object.
//
// See the Ntuple definition below for the full c'tor signatures.
//
// Template arguments
// ------------------
//
// The template arguments supplied for the Ntuple type indicate the
// type of the column.  The following Ntuple specifications are
// identical in semantics:
//
//   Ntuple<int, double, string> n1 {...};
//   Ntuple<column<int>, column<double>, column<string>> n2 {...};
//
// both of which indicate three columns with SQLite types of INTEGER,
// NUMERIC, and TEXT, respectively.  Specifying the 'column' template
// is necessary ONLY when a column constraint is desired (e.g.):
//
//   Ntuple<column<int, primary_key>, double, string> n3 {...};
//
// The names of the columns are provided as a c'tor argument (see
// below).
//
// Intended use
// ------------
//
// There are two public modifying functions that can be called:
//
//    Ntuple::insert(...)
//    Ntuple::flush()
//
// Calling 'insert' will add items to the internal buffer until the
// maximum buffer size has been reached, at which point, the contents
// of the buffer will be automatically flushed to the SQLite database.
//
// Calling 'flush' is necessary only when no more items will be
// inserted into the Ntuple AND querying of the Ntuple is required
// immediately thereafter.  The Ntuple d'tor automatically calls flush
// whenever the Ntuple object goes out of scope.
//
// Examples of use
// ---------------
//
//    using namespace cet::sqlite;
//
//    // Owning use case
//    Ntuple<string, int, int, int> bdays {"bdays.db", "birthdays", {"Name", "Day", "Month", "Year"}};
//    bdays.insert("Betty", 9, 24, 1947);
//    bdays.insert("David", 3, 12, 2015);
//
//    // Non-owning use case
//    Connection c {"languages.db"};
//    Ntuple<string, string> langs {c, "europe", {"Country","Language"}};
//    langs.insert("Germany", "German");
//    langs.insert("Switzerland", "German");
//    langs.insert("Switzerland", "Italian");
//    langs.insert("Switzerland", "French");
//    langs.insert("Switzerland", "Romansh");
//    langs.flush();
//
//    query_result<string> ch;
//    ch << select("Languange").from(c, "europe").where("Country='Switzerland'");
//    // see cet::sqlite::select documentation regarding using query_result.
//
// -----------------------------------------------------------
//
// Technical notes:
//
//   In principle, one could use a concurrent container to prevent
//   having to lock whenever an insert is done.  However, since a
//   flush occurs whenever the buffer max is reached, the buffer must
//   be protected from any modification until the flush is complete.
//   A lock is therefore inevitable.  We could probably optimize by
//   using an atomic variable to protect against modification only
//   when a flush is being done.  This is a potential optimization to
//   keep in mind.

// ===========================================================

#include "cetlib/sqlite/Transaction.h"
#include "cetlib/sqlite/column.h"
#include "cetlib/sqlite/detail/bind_parameters.h"
#include "cetlib/sqlite/helpers.h"

#include "sqlite3.h"

#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <tuple>
#include <vector>

namespace cet {
  namespace sqlite {

    template <typename T, typename... Constraints>
    using column = sqlite::column<T, Constraints...>;

    template <typename... Args>
    class Ntuple {
    public:

      // Elements of row are unique_ptr's so that it is possible to bind
      // to a null parameter.
      template <typename T>
      using element_t = std::unique_ptr<typename sqlite::permissive_column<T>::element_type>;

      using row_t = std::tuple<element_t<Args>...>;
      static constexpr auto nColumns = std::tuple_size<row_t>::value;
      using name_array = sqlite::name_array<nColumns>;

      // Non-owning version--does not call sqlite3_close on 'db' when
      // the d'tor is called.
      Ntuple(sqlite3* db,
             std::string const& name,
             name_array const& columns,
             bool overwriteContents = false,
             std::size_t bufsize = 1000ull);

      // Owning version--sqlite3_close is called on the internally owned
      // database connection handle.
      Ntuple(std::string const& filename,
             std::string const& tablename,
             name_array const& columns,
             bool overwriteContents = false,
             std::size_t bufsiz = 1000ull);

      ~Ntuple() noexcept;

      std::string const& name() const { return name_; }

      void insert(Args const...);
      void flush();

      // Disable copying
      Ntuple(Ntuple const&) = delete;
      Ntuple& operator=(Ntuple const&) = delete;

    private:

      static constexpr auto iSequence = std::make_index_sequence<nColumns>();

      // This is the c'tor that does all of the work.  It exists so that
      // the Args... and column-names array can be expanded in parallel.
      template <std::size_t... I>
      Ntuple(sqlite3* db,
             std::string const& name,
             name_array const& columns,
             bool overwriteContents,
             std::size_t bufsize,
             bool ownsConnection,
             std::index_sequence<I...>);

      int flush_no_throw();

      sqlite3* db_;
      std::string name_;
      std::size_t max_;
      bool ownsConnection_;
      std::vector<row_t> buffer_ {};
      sqlite3_stmt* insert_statement_ {nullptr};
      std::recursive_mutex mutex_ {};
    };

  } // sqlite
} // cet

template <typename... Args>
template <std::size_t... I>
cet::sqlite::Ntuple<Args...>::Ntuple(sqlite3* const db,
                                     std::string const& name,
                                     name_array const& cnames,
                                     bool const overwriteContents,
                                     std::size_t const bufsize,
                                     bool const ownsConnection,
                                     std::index_sequence<I...>) :
  db_{db},
  name_{name},
  max_{bufsize},
  ownsConnection_{ownsConnection}
{
  if (!db) {
    throw sqlite::Exception{sqlite::errors::SQLExecutionError}
    << "Attempt to create Ntuple with null database pointer.";
  }

  sqlite::createTableIfNeeded(db,
                              overwriteContents,
                              name,
                              sqlite::permissive_column<Args>{cnames[I]}...);

  std::string sql {"INSERT INTO "};
  sql += name;
  sql += " VALUES (?";
  for (std::size_t i = 1; i < nColumns; ++i) { sql += ",?"; }
  sql += ")";
  int const rc {sqlite3_prepare_v2(db_,
                                   sql.c_str(),
                                   sql.size(),
                                   &insert_statement_,
                                   nullptr)};
  if (rc != SQLITE_OK) {
    auto const ec = sqlite3_step(insert_statement_);
    throw sqlite::Exception{sqlite::errors::SQLExecutionError}
    << "Failed to prepare insertion statement.\n"
         << "Return code: " << ec << '\n';
  }

  buffer_.reserve(bufsize);
}

template <typename... Args>
cet::sqlite::Ntuple<Args...>::Ntuple(sqlite3* db,
                                     std::string const& name,
                                     name_array const& cnames,
                                     bool const overwriteContents,
                                     std::size_t const bufsize) :
  Ntuple{db, name, cnames, overwriteContents, bufsize, false, iSequence}
{}

template <typename... Args>
cet::sqlite::Ntuple<Args...>::Ntuple(std::string const& filename,
                                     std::string const& name,
                                     name_array const& cnames,
                                     bool const overwriteContents,
                                     std::size_t const bufsize) :
  Ntuple{sqlite::openDatabaseConnection(filename), name, cnames, overwriteContents, bufsize, true, iSequence}
{}

template <typename... Args>
cet::sqlite::Ntuple<Args...>::~Ntuple() noexcept
{
  if (flush_no_throw() != SQLITE_OK) {
    std::cerr << "SQLite step failure while flushing.\n";
  }
  sqlite3_finalize(insert_statement_);
  if (ownsConnection_) {
    sqlite3_close(db_);
  }
}

template <typename... Args>
void
cet::sqlite::Ntuple<Args...>::insert(Args const... args)
{
  std::lock_guard<decltype(mutex_)> lock {mutex_};
  if (buffer_.size() == max_) {
    flush();
  }
  buffer_.emplace_back(std::make_unique<Args>(args)...);
}

template <typename... Args>
int
cet::sqlite::Ntuple<Args...>::flush_no_throw()
{
  std::lock_guard<decltype(mutex_)> lock {mutex_};
  sqlite::Transaction txn {db_};
  for (auto const& r : buffer_) {
    sqlite::detail::bind_parameters<row_t, nColumns>::bind(insert_statement_, r);
    int const rc {sqlite3_step(insert_statement_)};
    if (rc != SQLITE_DONE) {
      return rc;
    }
    sqlite3_reset(insert_statement_);
  }
  txn.commit();
  buffer_.clear();
  return SQLITE_OK;
}

template <typename... Args>
void
cet::sqlite::Ntuple<Args...>::flush()
{
  // No lock here -- lock held by flush_no_throw;
  if (flush_no_throw() != SQLITE_OK) {
    throw sqlite::Exception{sqlite::errors::SQLExecutionError}
    << "SQLite step failure while flushing.";
  }
}

#endif /* cetlib_Ntuple_h */

// Local Variables:
// mode: c++
// End:
