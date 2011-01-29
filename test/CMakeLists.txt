# ======================================================================
#
# Testing
#
# ======================================================================

# ======================================================================
# Part 1: Categorize each source or data file
# ======================================================================

# ----------------------------------------------------------------------
# Identify libraries to be linked:

link_libraries( cetlib )

# ----------------------------------------------------------------------
# Identify each test's build requirements:

set( BUILD              # compile
  bit_test
  canonical_number_test
  canonical_string_test
  coded_exception_test
  container_algs_test
  cpu_timer_test
  exception_collector_test
  exception_test
  exempt_ptr_test
  filesystem_test
  getenv_test
  include_test
  nybbler_test
  registry_test
  registry_via_id_test
  registry_via_id_test_2 # for now -- see test's source
  search_path_test
  search_path_test_2
  sha1_test
  split_path_test
  split_test
  value_ptr_test
  value_ptr_test_2
)

set( BUILD_BOOST        # compile and link with BOOST_TEST library
  is_absolute_filepath_t
  search_path_test_3
)

set( HANDBUILT          # use target-specific build rules, found below
  sha1_test_2
)

set( PREBUILT           # no build required
  inc-expand_test.sh
  search_path_test_2.sh
)

# ----------------------------------------------------------------------
# Identify data files needed to run the above tests:

set( DATAFILE           # install only
)

# ----------------------------------------------------------------------
# Identify tests not to be run automatically:

set( HANDRUN            # use target-specific run rules, found below
  cpu_timer_test
  inc-expand_test
  inc-expand_test.sh
  registry_via_id_test_2 # for now -- see test's source
  search_path_test_2
)

# ======================================================================
# Part 2: Provide build instructions
# ======================================================================

# ----------------------------------------------------------------------
# BUILD:

foreach( test ${BUILD} )
  add_executable( ${test} ${test}.cc )
endforeach( test )

# ----------------------------------------------------------------------
# BUILD_BOOST:

foreach( test ${BUILD_BOOST} )
  add_executable( ${test} ${test}.cc )
  set_target_properties ( ${test} PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_MAIN
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
  )
  target_link_libraries( ${test} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} )
endforeach( test )

# ----------------------------------------------------------------------
# HANDBUILT:

# add_executable( ... )
# target_link_libraries( ... )

add_executable( sha1_test_performance sha1_test_performance.cc sha1.cpp )
target_link_libraries( sha1_test_performance cetlib )

add_executable( sha1_test_2 sha1_test_2.cc sha1.cpp )
target_link_libraries( sha1_test_2 cetlib )

# ----------------------------------------------------------------------
# PREBUILT:

foreach( test ${PREBUILT} )
  add_custom_target( ${test}_copy ALL
    COMMAND ${CMAKE_COMMAND} -E
      copy ${CMAKE_CURRENT_SOURCE_DIR}/${test}
           ${EXECUTABLE_OUTPUT_PATH}/
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${test}
  )
endforeach( test )

# ----------------------------------------------------------------------
# DATAFILE:

foreach( input ${DATAFILE} )
  add_custom_target( ${input}_copy ALL
    COMMAND ${CMAKE_COMMAND} -E
      copy ${CMAKE_CURRENT_SOURCE_DIR}/${input}
           ${CMAKE_CURRENT_BINARY_DIR}/
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${input}
  )
endforeach( input )

# ======================================================================
# Part 3: Run tests
# ======================================================================

# ----------------------------------------------------------------------
# Run each test not excepted in Part 1:

set( TESTS_TO_RUN ${BUILD} ${BUILD_BOOST} ${HANDBUILT} ${PREBUILT} )
list( REMOVE_ITEM TESTS_TO_RUN ${HANDRUN} )
foreach( test ${TESTS_TO_RUN} )
  add_test( ${test} ${EXECUTABLE_OUTPUT_PATH}/${test} )
endforeach( test )

# ----------------------------------------------------------------------
# HANDRUN:

add_test( inc-expand_test.sh
          ${EXECUTABLE_OUTPUT_PATH}/inc-expand_test.sh
          ${EXECUTABLE_OUTPUT_PATH}
)

# ======================================================================
