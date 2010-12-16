# ======================================================================
#
# Build/install script
#
# ======================================================================

# ----------------------------------------------------------------------
# Identify this library's name:

set( LIB_NAME ${PROJECT_NAME} )

# ----------------------------------------------------------------------
# Identify, by category, all files that comprise this library:

set( LIB_HEADERS
  bit_manipulation.h
  canonical_number.h
  canonical_string.h
  coded_exception.h
  container_algorithms.h
  cpu_timer.h
  exception.h
  exception_collector.h
  exempt_ptr.h
  filesystem.h
  getenv.h
  include.h
  nybbler.h
  registry.h
  search_path.h
  sha1.h
  split.h
  split_path.h
  value_ptr.h
)
set( LIB_SOURCES
  detail/polarssl_sha1.cc
  canonical_number.cc
  canonical_string.cc
  cpu_timer.cc
  exception.cc
  exception_collector.cc
  filesystem.cc
  getenv.cc
  include.cc
  nybbler.cc
  search_path.cc
  split_path.cc
)

# ----------------------------------------------------------------------
# Compile/link this library from the above-named sources:

link_libraries( ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY} )
add_library( ${LIB_NAME} SHARED ${LIB_SOURCES} )

# ----------------------------------------------------------------------
# Install the above-named headers and the above-built library:

install( FILES ${LIB_HEADERS}
         DESTINATION ${product}/${version}/include/${LIB_NAME} )
install( FILES detail/polarssl_config.h
               detail/polarssl_sha1.h
        DESTINATION ${product}/${version}/include/${LIB_NAME}/detail )
install( TARGETS ${LIB_NAME}
         DESTINATION ${flavorqual_dir}/lib )


#
# ======================================================================
