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
  _polarssl_config.h
  _polarssl_sha1.h
  bit_manipulation.h
  canonical_number.h
  canonical_string.h
  coded_exception.h
  container_algorithms.h
  cpu_timer.h
  exception.h
  exception_collector.h
  exempt_ptr.h
  nybbler.h
  registry.h
  search_path.h
  sha1.h
  split.h
  split_path.h
  value_ptr.h
)
set( LIB_SOURCES
  _polarssl_sha1.cc
  canonical_number.cc
  canonical_string.cc
  cpu_timer.cc
  exception.cc
  exception_collector.cc
  find_matching_files.cc
  nybbler.cc
  search_path.cc
  split_path.cc
)

# ----------------------------------------------------------------------
# Compile/link this library from the above-named sources:

link_libraries( ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY} )
add_library( ${LIB_NAME} SHARED ${LIB_SOURCES} )

build_dictionary ( cetlib )

# ----------------------------------------------------------------------
# Install the above-named headers and the above-built library:

install( FILES ${LIB_HEADERS}
         DESTINATION ${product}/${version}/include/${LIB_NAME} )
install( TARGETS ${LIB_NAME}
         DESTINATION ${flavorqual_dir}/lib )


#
# ======================================================================
