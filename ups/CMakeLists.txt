# ======================================================================
#
# Build/install script for ups files
#
# ======================================================================


# ----------------------------------------------------------------------
# Record time stamp:

execute_process( COMMAND date
                 OUTPUT_VARIABLE datime
                 OUTPUT_STRIP_TRAILING_WHITESPACE )

# ----------------------------------------------------------------------
# Generate ups files from prototypes:

configure_file( ${cetlib_SOURCE_DIR}/ups/cetlib.table.in
                ${cetlib_BINARY_DIR}/ups/cetlib.table @ONLY )
configure_file( ${cetlib_SOURCE_DIR}/ups/cetlib.version.in
                ${cetlib_BINARY_DIR}/ups/${version}.version @ONLY )

# ----------------------------------------------------------------------
# Install generated files:

install(FILES ${cetlib_BINARY_DIR}/ups/cetlib.table
        DESTINATION ${version}/ups)
install(FILES ${cetlib_BINARY_DIR}/ups/${version}.version
        DESTINATION ${CMAKE_INSTALL_PREFIX} )

#
# ======================================================================
