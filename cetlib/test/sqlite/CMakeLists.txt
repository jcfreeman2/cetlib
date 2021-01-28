cet_test(connection_t SCOPED LIBRARIES PRIVATE Threads::Threads cetlib::cetlib)
cet_test(create_table_ddl_t SCOPED LIBRARIES PRIVATE cetlib::cetlib)
cet_test(insert_t SCOPED)
cet_test(normalize_statement_t SCOPED LIBRARIES PRIVATE Threads::Threads cetlib::cetlib)
cet_test(ntuple_t SCOPED LIBRARIES PRIVATE Threads::Threads cetlib::cetlib)
cet_test(query_result_t SCOPED LIBRARIES PRIVATE cetlib::cetlib)
cet_test(statistics_t SCOPED LIBRARIES PRIVATE cetlib::cetlib)
cet_test(transaction_t SCOPED
  LIBRARIES PRIVATE cetlib::cetlib
  TEST_PROPERTIES
  PASS_REGULAR_EXPRESSION "Transaction d'tor called before commit was called.")
