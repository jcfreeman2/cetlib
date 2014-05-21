# ======================================================================
#
# Build/install script
#
# ======================================================================

include(CetMake)

# ----------------------------------------------------------------------
# Build and install this project's library:

set( LIB_NAME ${PROJECT_NAME} )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/shlib_utils.cc.in
  ${CMAKE_CURRENT_BINARY_DIR}/shlib_utils.cc @ONLY
  )

cet_make_library(LIBRARY_NAME ${LIB_NAME}
  SOURCE
  ${CMAKE_CURRENT_BINARY_DIR}/shlib_utils.cc
  ../polarssl/md5.cc
  ../polarssl/sha1.cc
  base_converter.cc
  canonical_number.cc
  canonical_string.cc
  cpu_timer.cc
  crc32.cc
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
  simple_stats.cc
  split_path.cc
  LibraryManager.cc
  LIBRARIES
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_REGEX_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  -ldl
  )


# ----------------------------------------------------------------------
# Build and install this project's executables:

cet_make_exec( inc-expand
               LIBRARIES ${LIB_NAME} ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY})

# ----------------------------------------------------------------------
# Install this project's headers and sources:

install_headers()
install_source()

#
# ======================================================================
