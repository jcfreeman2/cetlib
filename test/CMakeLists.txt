# ======================================================================
#
# Testing script
#
# ======================================================================

# ----------------------------------------------------------------------
# Identify tests:

set( TEST_LIST
  bit_test
  coded_exception_test
  container_algs_test
  exception_test
  exception_collector_test
  exempt_ptr_test
  nybbler_test
  registry_test
  search_path_test
  sha1_test
  split_test
  value_ptr_test
  value_ptr_test_2
)

set( ALSO_BUILD
  cpu_timer_test
  find_matching_files_test
)

set( ALSO_RUN
  sha1_test_2
  find_matching_files_test.sh
)

# ----------------------------------------------------------------------
# Build most tests:

foreach( test ${TEST_LIST} ${ALSO_BUILD} )
  add_executable( ${test} ${test}.cc )
  target_link_libraries( ${test} cetlib )
endforeach( test ${TEST_LIST} ${ALSO_BUILD} )

# ----------------------------------------------------------------------
# Build tests with special requirements:

add_executable( sha1_test_performance sha1_test_performance.cc sha1.cpp )
target_link_libraries( sha1_test_performance cetlib )

add_executable( sha1_test_2 sha1_test_2.cc sha1.cpp )
target_link_libraries( sha1_test_2 cetlib )

# install( FILES find_matching_files_test.sh DESTINATION test  )
# add_test( pigface find_matching_files_test.sh )

# ----------------------------------------------------------------------
# Run tests:

foreach( test ${TEST_LIST} ${ALSO_RUN} )
  add_test( ${test} ${EXECUTABLE_OUTPUT_PATH}/${test} )
endforeach( test ${TEST_LIST} )

# ======================================================================
