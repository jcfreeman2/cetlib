#ifndef cetlib_Ntuple_Ntuple_h
#define cetlib_Ntuple_Ntuple_h

#include <array>
#include "cetlib/Ntuple/sqlite_helpers.h"
#include "cetlib/Ntuple/Transaction.h"
#include "cetlib/sqlite/bind_parameters.h"
#include "cetlib/sqlite/column.h"
#include "cetlib/sqlite/insert.h"

#include "sqlite3.h"

#include <memory>
#include <string>
#include <tuple>
#include <vector>

namespace cet {

  template <class ... ARGS>
  class Ntuple {
  public:

    // Disable copying
    Ntuple(Ntuple const&) = delete;
    Ntuple& operator=(Ntuple const&) = delete;

    using row_t = std::tuple<std::unique_ptr<ARGS>...>;
    static constexpr auto SIZE = std::tuple_size<row_t>::value;

    template <size_t N>
    using name_array = sqlite::name_array<N>;

    Ntuple(sqlite3* db,
           std::string const& name,
           name_array<SIZE> const& columns,
           bool overwriteContents = false,
           std::size_t bufsize = 1000ull);

    Ntuple(std::string const& filename,
           std::string const& tablename,
           name_array<SIZE> const& columns,
           bool overwriteContents = false,
           std::size_t bufsiz = 1000ull);

    ~Ntuple() noexcept;

    void insert(ARGS const...);
    std::string const& name() const { return name_; }
    void flush();
    sqlite3_int64 lastRowid() const { return last_rowid_; }

  private:

    int flush_no_throw();

    sqlite3* db_;
    std::string name_;
    std::size_t max_;
    std::vector<row_t> buffer_ {};
    sqlite3_stmt* insert_statement_ {nullptr};
    sqlite3_int64 last_rowid_ {};
  };

}

template <class ...ARGS>
cet::Ntuple<ARGS...>::Ntuple(sqlite3* db,
                             std::string const& name,
                             name_array<SIZE> const& cnames,
                             bool const overwriteContents,
                             std::size_t bufsize) :
  db_{db},
  name_{name},
  max_{bufsize}
{
  if (!db) {
    throw sqlite::Exception{sqlite::errors::SQLExecutionError,"Attempt to create Ntuple with null database pointer"};
  }

  sqlite::createTableIfNeeded(db,
                              last_rowid_,
                              name,
                              sqlite::make_column_pack<ARGS...>(cnames, std::index_sequence_for<ARGS...>()),
                              overwriteContents);

  std::string sql {"INSERT INTO "};
  sql += name;
  sql += " VALUES (?";
  for (std::size_t i = 1; i < SIZE; ++i) { sql += ",?"; }
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

template <class ...ARGS>
cet::Ntuple<ARGS...>::Ntuple(std::string const& filename,
                             std::string const& name,
                             name_array<SIZE> const& cnames,
                             bool const overwriteContents,
                             std::size_t bufsize) :
  Ntuple{sqlite::openDatabaseFile(filename), name, cnames, overwriteContents, bufsize}
{}

template <class ... ARGS>
cet::Ntuple<ARGS...>::~Ntuple() noexcept
{
  int const rc = flush_no_throw();
  if (rc != SQLITE_OK) {
    std::cerr << "SQLite step failure while flushing";
  }
  sqlite3_finalize(insert_statement_);
}

template <class ...ARGS>
void
cet::Ntuple<ARGS...>::insert(ARGS const... args)
{
  if (buffer_.size() == max_) {
    flush();
  }
  buffer_.emplace_back(std::make_unique<ARGS>(args)...);
  ++last_rowid_;
}

template <class ...ARGS>
int
cet::Ntuple<ARGS...>::flush_no_throw()
{
  sqlite::Transaction txn {db_};
  for (auto const& r : buffer_) {
    sqlite::detail::bind_parameters<row_t, SIZE>::bind(insert_statement_, r);
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

template <class ...ARGS>
void
cet::Ntuple<ARGS...>::flush()
{
  if (flush_no_throw() != SQLITE_OK) {
    throw sqlite::Exception{sqlite::errors::SQLExecutionError, "SQLite step failure while flushing"};
  }
}

#endif /* cetlib_Ntuple_Ntuple_h */

// Local Variables:
// mode: c++
// End:
