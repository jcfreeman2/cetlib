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
  bit_manipulation.h
  container_algorithms.h
  exempt_ptr.h
  nybbler.h
  polarssl_config.h
  polarssl_sha1.h
  registry.h
  sha1.h
)
set( LIB_SOURCES
  nybbler.cc
  polarssl_sha1.cc
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
