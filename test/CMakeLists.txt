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
  split_path_test
  split_test
  value_ptr_test
  value_ptr_test_2
)

set( ALSO_BUILD
  cpu_timer_test
  find_matching_files_test
)

set( MANUALLY_BUILT_TESTS
  sha1_test_2
)

set ( ALSO_RUN
)

set( MANUALLY_RUN_TESTS
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

# ----------------------------------------------------------------------
# Install non-built tests
foreach ( script ${ALSO_RUN} ${MANUALLY_RUN_TESTS} )
  add_custom_target ( ${script} ALL
                      COMMAND ${CMAKE_COMMAND} -E 
                        copy ${CMAKE_CURRENT_SOURCE_DIR}/${script}
                             ${EXECUTABLE_OUTPUT_PATH}/
                       DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${script}
                    )
endforeach ( script ${TEST_SCRIPTS} )

# ----------------------------------------------------------------------
# Run tests:

foreach( test ${TEST_LIST} ${MANUALLY_BUILT_TESTS} )
  add_test( ${test} ${EXECUTABLE_OUTPUT_PATH}/${test} )
endforeach( test ${TEST_LIST} )

# Custom tests:
add_test ( find_matching_files_test.sh
           ${EXECUTABLE_OUTPUT_PATH}/find_matching_files_test.sh
           ${EXECUTABLE_OUTPUT_PATH}
         )


# ======================================================================
