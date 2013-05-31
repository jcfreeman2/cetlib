# ======================================================================
#
# Build/install script
#
# ======================================================================

include(CetMake)

# ----------------------------------------------------------------------
# Build and install this project's library:

set( LIB_NAME ${PROJECT_NAME} )

cet_make( LIBRARY_NAME ${LIB_NAME}
          EXCLUDE inc-expand.cc
          SUBDIRS ../polarssl
          LIBRARIES ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY} )

# ----------------------------------------------------------------------
# Build and install this project's executables:

cet_make_exec( inc-expand
               LIBRARIES ${LIB_NAME} ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY} )

# ----------------------------------------------------------------------
# Install this project's headers and sources:

install_headers()
install_source()

#
# ======================================================================
