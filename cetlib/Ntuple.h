#ifndef cetlib_Ntuple_Ntuple_h
#define cetlib_Ntuple_Ntuple_h

#include "cetlib/sqlite/Transaction.h"
#include "cetlib/sqlite/bind_parameters.h"
#include "cetlib/sqlite/column.h"
#include "cetlib/sqlite/helpers.h"
#include "cetlib/sqlite/insert.h"

#include "sqlite3.h"

#include <array>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

namespace cet {

  template <typename... Args>
  class Ntuple {
  public:

    // Disable copying
    Ntuple(Ntuple const&) = delete;
    Ntuple& operator=(Ntuple const&) = delete;

    using row_t = std::tuple<std::unique_ptr<Args>...>;
    static constexpr auto nColumns = std::tuple_size<row_t>::value;
    static constexpr auto iSequence = std::make_index_sequence<nColumns>();

    template <std::size_t N>
    using name_array = sqlite::name_array<N>;

    Ntuple(sqlite3* db,
           std::string const& name,
           name_array<nColumns> const& columns,
           bool overwriteContents = false,
           std::size_t bufsize = 1000ull);

    Ntuple(std::string const& filename,
           std::string const& tablename,
           name_array<nColumns> const& columns,
           bool overwriteContents = false,
           std::size_t bufsiz = 1000ull);

    ~Ntuple() noexcept;

    void insert(Args const...);
    std::string const& name() const { return name_; }
    void flush();
    sqlite3_int64 lastRowid() const { return last_rowid_; }

  private:

    // This is the c'tor that does all of the work.  It exists so that
    // the Args... and column-names array can be expanded in parallel.
    template <size_t... I>
    Ntuple(sqlite3* db,
           std::string const& name,
           name_array<nColumns> const& columns,
           bool overwriteContents = false,
           std::size_t bufsize = 1000ull,
           std::index_sequence<I...> = iSequence);

    int flush_no_throw();

    sqlite3* db_;
    std::string name_;
    std::size_t max_;
    std::vector<row_t> buffer_ {};
    sqlite3_stmt* insert_statement_ {nullptr};
    sqlite3_int64 last_rowid_ {};
  };

}

template <class... Args>
template <std::size_t... I>
cet::Ntuple<Args...>::Ntuple(sqlite3* db,
                             std::string const& name,
                             name_array<nColumns> const& cnames,
                             bool const overwriteContents,
                             std::size_t const bufsize,
                             std::index_sequence<I...>) :
  db_{db},
  name_{name},
  max_{bufsize}
{
  if (!db) {
    throw sqlite::Exception{sqlite::errors::SQLExecutionError,"Attempt to create Ntuple with null database pointer"};
  }

  sqlite::createTableIfNeeded(db,
                              last_rowid_,
                              overwriteContents,
                              name,
                              sqlite::column<Args>{cnames[I]}...);

  std::string sql {"INSERT INTO "};
  sql += name;
  sql += " VALUES (?";
  for (std::size_t i = 1; i < nColumns; ++i) { sql += ",?"; }
  sql += ")";
  int const rc = sqlite3_prepare_v2(db_,
                                    sql.c_str(),
                                    sql.size(),
                                    &insert_statement_,
                                    nullptr);
  if (rc != SQLITE_OK) {
    auto const ec = sqlite3_step(insert_statement_);
    throw sqlite::Exception{sqlite::errors::SQLExecutionError}
    << "Failed to prepare insertion statement.\n"
    << "Return code: " << ec << '\n';
  }

  buffer_.reserve(bufsize);
}

template <typename... Args>
cet::Ntuple<Args...>::Ntuple(sqlite3* db,
                             std::string const& name,
                             name_array<nColumns> const& cnames,
                             bool const overwriteContents,
                             std::size_t const bufsize) :
  Ntuple{db, name, cnames, overwriteContents, bufsize, iSequence}
{}

template <typename... Args>
cet::Ntuple<Args...>::Ntuple(std::string const& filename,
                             std::string const& name,
                             name_array<nColumns> const& cnames,
                             bool const overwriteContents,
                             std::size_t const bufsize) :
  Ntuple{sqlite::openDatabaseFile(filename), name, cnames, overwriteContents, bufsize, iSequence}
{}

template <typename... Args>
cet::Ntuple<Args...>::~Ntuple() noexcept
{
  int const rc = flush_no_throw();
  if (rc != SQLITE_OK) {
    std::cerr << "SQLite step failure while flushing";
  }
  sqlite3_finalize(insert_statement_);
}

template <typename... Args>
void
cet::Ntuple<Args...>::insert(Args const... args)
{
  if (buffer_.size() == max_) {
    flush();
  }
  buffer_.emplace_back(std::make_unique<Args>(args)...);
  ++last_rowid_;
}

template <typename... Args>
int
cet::Ntuple<Args...>::flush_no_throw()
{
  sqlite::Transaction txn {db_};
  for (auto const& r : buffer_) {
    sqlite::detail::bind_parameters<row_t, nColumns>::bind(insert_statement_, r);
    int const rc = sqlite3_step(insert_statement_);
    if (rc != SQLITE_DONE) {
      return rc;
    }
    last_rowid_ = sqlite3_last_insert_rowid(db_);
    sqlite3_reset(insert_statement_);
  }
  txn.commit();
  buffer_.clear();
  return SQLITE_OK;
}

template <typename... Args>
void
cet::Ntuple<Args...>::flush()
{
  if (flush_no_throw() != SQLITE_OK) {
    throw sqlite::Exception{sqlite::errors::SQLExecutionError, "SQLite step failure while flushing"};
  }
}

#endif /* cetlib_Ntuple_Ntuple_h */

// Local Variables:
// mode: c++
// End:
