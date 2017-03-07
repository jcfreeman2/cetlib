#ifndef cetlib_sqlite_Transaction_h
#define cetlib_sqlite_Transaction_h

// ====================================================================
// Transaction
//
// A Transaction object encapsulates a single SQLite transaction. Such
// transactions can not be nested. Look into using SAVEPOINT if you
// need nesting.
//
// ===================================================================

struct sqlite3;

namespace cet {
  namespace sqlite {

    class Transaction {
    public:

      // Creating a Transaction begins a transaction in the given
      // db. It is expected that the database has already be opened.
      explicit Transaction(sqlite3* db);

      // Transactions may not be copied or moved.
      Transaction(Transaction const&) = delete;
      Transaction(Transaction&&) = delete;
      Transaction& operator=(Transaction const&) = delete;
      Transaction& operator=(Transaction&&) = delete;

      // Destroying the Transaction will roll back the associated
      // transaction, unless 'commit' has been called.
      ~Transaction() noexcept;

      // Commit the associated SQLite transaction. Unless committed,
      // the transaction will be rolled back upon
      // destruction. 'commit' may only be called once per
      // Transaction.
      void commit();

    private:
      sqlite3* db_;
    };

  } // sqlite
} // cet

#endif /* cetlib_sqlite_Transaction_h */

// Local Variables:
// mode: c++
// End:
