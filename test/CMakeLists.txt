# ======================================================================
#
# Build/install script
#
# ======================================================================


# ----------------------------------------------------------------------
# Test bit_manipulation component:

add_executable( bit_test bit_test.cc )
target_link_libraries( bit_test cetlib )
add_test( bit_test ${EXECUTABLE_OUTPUT_PATH}/bit_test )


# ----------------------------------------------------------------------
# Test coded_exception component:

add_executable( coded_exception_test coded_exception_test.cc )
target_link_libraries( coded_exception_test cetlib )
add_test( coded_exception_test ${EXECUTABLE_OUTPUT_PATH}/coded_exception_test )


# ----------------------------------------------------------------------
# Test container_algorithms component:

add_executable( container_algs_test container_algs_test.cc )
target_link_libraries( container_algs_test cetlib )
add_test( container_algs_test ${EXECUTABLE_OUTPUT_PATH}/container_algs_test )


# ----------------------------------------------------------------------
# Test exception component:

add_executable( exception_test exception_test.cc )
target_link_libraries( exception_test cetlib )
add_test( exception_test ${EXECUTABLE_OUTPUT_PATH}/exception_test )


# ----------------------------------------------------------------------
# Test exception_collector component:

add_executable( exception_collector_test exception_collector_test.cc )
target_link_libraries( exception_collector_test cetlib )
add_test( exception_collector_test
${EXECUTABLE_OUTPUT_PATH}/exception_collector_test )


# ----------------------------------------------------------------------
# Test exempt_ptr component:

add_executable( exempt_ptr_test exempt_ptr_test.cc )
target_link_libraries( exempt_ptr_test cetlib )
add_test( exempt_ptr_test ${EXECUTABLE_OUTPUT_PATH}/exempt_ptr_test )


# ----------------------------------------------------------------------
# Test nybbler component:

add_executable( nybbler_test nybbler_test.cc )
target_link_libraries( nybbler_test cetlib )
add_test( nybbler_test ${EXECUTABLE_OUTPUT_PATH}/nybbler_test )


# ----------------------------------------------------------------------
# Test registry component:

add_executable( registry_test registry_test.cc )
target_link_libraries( registry_test cetlib )
add_test( registry_test ${EXECUTABLE_OUTPUT_PATH}/registry_test )


# ----------------------------------------------------------------------
# Test search_path component:

add_executable( search_path_test search_path_test.cc )
target_link_libraries( search_path_test cetlib )
add_test( search_path_test ${EXECUTABLE_OUTPUT_PATH}/search_path_test )


# ----------------------------------------------------------------------
# Test sha1 component:

add_executable( sha1_test sha1_test.cc )
target_link_libraries( sha1_test cetlib )
add_test( sha1_test ${EXECUTABLE_OUTPUT_PATH}/sha1_test )


# ----------------------------------------------------------------------
# Test_2 sha1 component:

add_executable( sha1_test_2 sha1_test_2.cc sha1.cpp )
target_link_libraries( sha1_test_2 cetlib )
add_test( sha1_test_2 ${EXECUTABLE_OUTPUT_PATH}/sha1_test_2 )


# ----------------------------------------------------------------------
# Test sha1 performance:

add_executable( sha1_test_performance sha1_test_performance.cc sha1.cpp )
target_link_libraries( sha1_test_performance cetlib )
add_test( sha1_test_performance
          ${EXECUTABLE_OUTPUT_PATH}/sha1_test_performance )


# ----------------------------------------------------------------------
# Test split component:

add_executable( split_test split_test.cc )
target_link_libraries( split_test cetlib )
add_test( split_test ${EXECUTABLE_OUTPUT_PATH}/split_test )


# ----------------------------------------------------------------------
# Test value_ptr component:

add_executable( value_ptr_test value_ptr_test.cc )
target_link_libraries( value_ptr_test cetlib )
add_test( value_ptr_test ${EXECUTABLE_OUTPUT_PATH}/value_ptr_test )

#
# ======================================================================
