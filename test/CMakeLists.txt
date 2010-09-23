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
# Test container_algorithms component:

add_executable( container_algs_test container_algs_test.cc )
target_link_libraries( container_algs_test cetlib )
add_test( container_algs_test ${EXECUTABLE_OUTPUT_PATH}/container_algs_test )


# ----------------------------------------------------------------------
# Test nybbler component:

add_executable( nybbler_test nybbler_test.cc )
target_link_libraries( nybbler_test cetlib )
add_test( nybbler_test ${EXECUTABLE_OUTPUT_PATH}/nybbler_test )


# ----------------------------------------------------------------------
# Test sha1 component:

add_executable( sha1_test sha1_test.cc )
target_link_libraries( sha1_test cetlib )
add_test( sha1_test ${EXECUTABLE_OUTPUT_PATH}/sha1_test )

#
# ======================================================================
