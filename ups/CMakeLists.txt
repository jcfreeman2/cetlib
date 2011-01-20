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
                ${cetlib_BINARY_DIR}/ups/${UPSFLAVOR}_${qualifier}  @ONLY )

# ----------------------------------------------------------------------
# Install generated files:

install( FILES ${cetlib_BINARY_DIR}/ups/cetlib.table
         DESTINATION ${product}/${version}/ups )
install( FILES ${cetlib_BINARY_DIR}/ups/${UPSFLAVOR}_${qualifier} 
         DESTINATION ${product}/${version}.version )

#
# ======================================================================
