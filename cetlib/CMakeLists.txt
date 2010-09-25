# ======================================================================
#
# Build/install script
#
# ======================================================================


# ----------------------------------------------------------------------
# Identify this library's name:

set( LIB_NAME ${PROJECT_NAME} )

# ----------------------------------------------------------------------
# Name, by category, all files that comprise this library:

set( LIB_HEADERS
  _polarssl_config.h
  _polarssl_sha1.h
  bit_manipulation.h
  container_algorithms.h
  exempt_ptr.h
  nybbler.h
  registry.h
  sha1.h
  value_ptr.h
)
set( LIB_SOURCES
  nybbler.cc
  _polarssl_sha1.cc
)


# ----------------------------------------------------------------------
# Compile/link this library from the above-named sources:

add_library( ${LIB_NAME} SHARED ${LIB_SOURCES} )
#link_libraries( ${Boost_FILESYSTEM_LIBRARY}
#                ${REFLEX}
#                dl )

# ----------------------------------------------------------------------
# Install the above-named headers and the above-built library:

install( FILES ${LIB_HEADERS}
         DESTINATION ${version}/include/${LIB_NAME} )
install( TARGETS ${LIB_NAME}
         DESTINATION ${flavorqual_dir}/lib )

#
# ======================================================================
