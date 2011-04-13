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
  column_width.h
  container_algorithms.h
  cpu_timer.h
  demangle.h
  exception.h
  exception_collector.h
  exempt_ptr.h
  filepath_maker.h
  filesystem.h
  getenv.h
  include.h
  includer.h
  lpad.h
  maybe_ref.h
  nybbler.h
  registry.h
  registry_via_id.h
  rpad.h
  search_path.h
  sha1.h
  split.h
  split_path.h
  value_ptr.h
  zero_init.h
)
set( LIB_SOURCES
  ../polarssl/sha1.cc
  canonical_number.cc
  canonical_string.cc
  cpu_timer.cc
  demangle.cc
  exception.cc
  exception_collector.cc
  filepath_maker.cc
  filesystem.cc
  getenv.cc
  include.cc
  includer.cc
  lpad.cc
  nybbler.cc
  rpad.cc
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
install( FILES ../polarssl/config.h
               ../polarssl/version.h
               ../polarssl/sha1.h
        DESTINATION ${product}/${version}/include/polarssl )
install( TARGETS ${LIB_NAME}
         DESTINATION ${flavorqual_dir}/lib )

# ----------------------------------------------------------------------
# Build and install executables:

add_executable( inc-expand inc-expand.cc )
target_link_libraries( inc-expand ${LIB_NAME} )
install( TARGETS inc-expand
         DESTINATION ${flavorqual_dir}/bin )

#
# ======================================================================
