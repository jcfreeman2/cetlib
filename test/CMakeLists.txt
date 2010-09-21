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
# Test nybbler component:

add_executable( nybbler_test nybbler_test.cc )
target_link_libraries( nybbler_test cetlib )
add_test( nybbler_test ${EXECUTABLE_OUTPUT_PATH}/nybbler_test )

#
# ======================================================================
