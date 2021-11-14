# ======================================================================
#
# Build/install script
#
# ======================================================================

include(CetMake)

find_package(Boost PRIVATE COMPONENTS regex REQUIRED)
find_package(Boost COMPONENTS program_options filesystem REQUIRED PUBLIC)
find_package(cetlib_except REQUIRED PUBLIC)
find_package(SQLite3 REQUIRED PUBLIC)
find_package(hep_concurrency REQUIRED PUBLIC)

####################################
# Libraries.
cet_make_library(LIBRARY_NAME container_algorithms INTERFACE
  SOURCE container_algorithms.h)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/shlib_utils.cc.in
  ${CMAKE_CURRENT_BINARY_DIR}/shlib_utils.cc @ONLY
)

cet_make_library(HEADERS_TARGET
  SOURCE
    ${CMAKE_CURRENT_BINARY_DIR}/shlib_utils.cc
    MD5Digest.cc
    BasicPluginFactory.cc
    LibraryManager.cc
    PluginFactory.cc
    base_converter.cc
    canonical_number.cc
    canonical_string.cc
    column_width.cc
    cpu_timer.cc
    crc32.cc
    detail/plugin_search_path.cc
    detail/wrapLibraryManagerException.cc
    filepath_maker.cc
    filesystem.cc
    getenv.cc
    include.cc
    includer.cc
    loadable_libraries.cc
    lpad.cc
    nybbler.cc
    replace_all.cc
    rpad.cc
    search_path.cc
    sha1.cc
    simple_stats.cc
    split_path.cc
    split_by_regex.cc
  LIBRARIES
    PUBLIC
      cetlib_except::cetlib_except
      cetlib::container_algorithms
    PRIVATE
      Boost::filesystem
      Boost::regex
      ${CMAKE_DL_LIBS}
)

# find openSSL
# when building on macOS, use CommonCrypto
if(NOT APPLE)
  find_package(OpenSSL REQUIRED PUBLIC)
  target_link_libraries(cetlib PUBLIC OpenSSL::Crypto)
endif()

cet_make_library(LIBRARY_NAME compiler_macros INTERFACE
  SOURCE compiler_macros.h)

cet_register_export_set(SET_NAME PluginSupport NAMESPACE art_plugin_support)

cet_make_library(LIBRARY_NAME support_macros INTERFACE
  EXPORT_SET PluginSupport
  SOURCE
    ProvideFilePathMacro.h
    ProvideMakePluginMacros.h
    PluginTypeDeducer.h
  LIBRARIES INTERFACE
    cetlib::compiler_macros
    Boost::filesystem
)

cet_make_library(LIBRARY_NAME cetlib_parsed_program_options
  SOURCE parsed_program_options.cc
  LIBRARIES
    PUBLIC Boost::program_options
    PRIVATE cetlib_except::cetlib_except
)
####################################

# Executables.
cet_make_exec(NAME inc-expand LIBRARIES PRIVATE cetlib::cetlib)
cet_make_exec(NAME get-accessible-paths LIBRARIES PRIVATE cetlib::cetlib Boost::filesystem)

# Headers and source.
install_headers(SUBDIRS detail)
install_source(SUBDIRS detail)

# SQLite functionality.
add_subdirectory(sqlite)

# Tests
add_subdirectory(test)
