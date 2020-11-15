# ======================================================================
#
# Testing
#
# ======================================================================

include(CetTest)
cet_enable_asserts()

cet_find_package(Threads REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

add_subdirectory(sqlite)

cet_test(bit_test)
cet_test(exempt_ptr_test)
cet_test(value_ptr_test_2)
cet_test(value_ptr_test_3)
cet_test(value_ptr_test_4)
cet_test(value_ptr_test_5)

set(TESTS_REQUIRING_BOOST_UNIT
    container_algs_test
    maybe_ref_test
    ostream_handle_test
    split_test
    trim_test
    value_ptr_test)
foreach(test IN LISTS TESTS_REQUIRING_BOOST_UNIT)
  cet_test(${test} USE_BOOST_UNIT)
endforeach()

set(TESTS_REQUIRING_CETLIB
    base_converter_test
    filesystem_test
    include_test
    MD5Digest_test
    nybbler_test
    pow_constexpr_test
    registry_test
    registry_via_id_test
    sha1_test)
foreach(test IN LISTS TESTS_REQUIRING_CETLIB)
  cet_test(${test} LIBRARIES PRIVATE cetlib::cetlib)
endforeach()

set(TESTS_REQUIRING_BOOST_UNIT_AND_CETLIB
    canonical_string_test
    column_width_test
    includer_test
    is_absolute_filepath_t
    lpad_test
    map_vector_test
    pow_test
    rpad_test
    search_path_test_3
    shlib_utils_t
    simple_stats_t
    split_by_regex_test)
foreach(test IN LISTS TESTS_REQUIRING_BOOST_UNIT_AND_CETLIB)
  cet_test(${test} USE_BOOST_UNIT LIBRARIES PRIVATE cetlib::cetlib)
endforeach()

cet_test(cpu_timer_test USE_BOOST_UNIT
  TEST_PROPERTIES RUN_SERIAL true
  OPTIONAL_GROUPS LOAD_SENSITIVE
  LIBRARIES PRIVATE cetlib::cetlib)
cet_test(filepath_maker_test USE_BOOST_UNIT
  TEST_PROPERTIES
  ENVIRONMENT "CURRENT_DIR=${CMAKE_CURRENT_SOURCE_DIR};\
FILEPATH_MAKER_TEST_FILES=${CMAKE_CURRENT_SOURCE_DIR}/filepath_maker-files"
  DATAFILES filepath_maker_test.txt
  LIBRARIES PRIVATE cetlib::cetlib)

cet_test(inc-expand_test.sh PREBUILT DEPENDENCIES inc-expand)

cet_test(registry_via_id_test_2 LIBRARIES PRIVATE cetlib::cetlib NO_AUTO) # for now -- see test's source
cet_test(search_path_test_2 NO_AUTO LIBRARIES PRIVATE cetlib::cetlib)
cet_test(search_path_test_2.sh PREBUILT DEPENDENCIES search_path_test_2)

cet_test(sha1_test_2 SOURCE sha1_test_2.cc sha1.cpp LIBRARIES PRIVATE cetlib::cetlib)
cet_test(sha1_test_performance NO_AUTO SOURCE sha1_test_performance.cc sha1.cpp LIBRARIES PRIVATE cetlib::cetlib)
cet_test(split_path_test LIBRARIES PRIVATE cetlib::cetlib)
cet_test(plugin_search_path_t USE_CATCH2_MAIN LIBRARIES PRIVATE Catch2::Catch2 cetlib::cetlib)

cet_make_library(LIBRARY_NAME cetlib_test_fakePlugin SOURCE moduleType.cc NO_INSTALL)
cet_make_library(LIBRARY_NAME cetlib_test_TestPluginBase SOURCE TestPluginBase.cc NO_INSTALL)

include(BasicPlugin)
basic_plugin(TestPlugin "plugin" NO_INSTALL LIBRARIES PRIVATE cetlib::cetlib_test_TestPluginBase)

# Use default Plugin lookup
cet_test(PluginFactory_t USE_BOOST_UNIT
  LIBRARIES PRIVATE cetlib::cetlib_test_TestPluginBase cetlib::cetlib)

# Use custom Plugin lookup
cet_test(PluginFactoryCustomSearchPath_t USE_BOOST_UNIT
  SOURCE PluginFactory_t.cc
  LIBRARIES PRIVATE cetlib::cetlib_test_TestPluginBase cetlib::cetlib
  TEST_PROPERTIES ENVIRONMENT PLUGIN_FACTORY_SEARCH_PATH=$<TARGET_FILE_DIR:cetlib::cetlib_test_TestPlugin_plugin>)
target_compile_definitions(PluginFactoryCustomSearchPath_t PRIVATE PLUGIN_FACTORY_SEARCH_PATH=1)

function(test_library LIBSPEC)
  add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${LIBSPEC}.cc
    COMMAND ${CMAKE_COMMAND}
    -DSRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
    -DBIN_DIR="${CMAKE_CURRENT_BINARY_DIR}"
    -DLIBSPEC="${LIBSPEC}"
    -P ${CMAKE_CURRENT_SOURCE_DIR}/configureLibraryManagerTestFunc.cmake
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/configureLibraryManagerTestFunc.cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/LibraryManagerTestFunc.cc.in)
  add_library(${LIBSPEC}_cetlibtest SHARED ${CMAKE_CURRENT_BINARY_DIR}/${LIBSPEC}.cc)
  target_link_libraries(${LIBSPEC}_cetlibtest PRIVATE cetlib::cetlib)
endfunction()

test_library(1_1_1)
test_library(1_1_2)
test_library(1_1_3)
test_library(1_2_3)
test_library(2_1_5)

# Use default library search path
cet_test(LibraryManager_t USE_BOOST_UNIT
  LIBRARIES PRIVATE
  cetlib::cetlib
  Boost::filesystem
  ${CMAKE_DL_LIBS})

# Use custom library search path
cet_test(LibraryManagerCustomSearchPath_t USE_BOOST_UNIT
  SOURCE LibraryManager_t.cc
  LIBRARIES PRIVATE
  cetlib::cetlib
  Boost::filesystem
  ${CMAKE_DL_LIBS}
  TEST_PROPERTIES ENVIRONMENT LIBRARY_MANAGER_SEARCH_PATH=$<TARGET_FILE_DIR:1_1_1_cetlibtest>)
target_compile_definitions(LibraryManagerCustomSearchPath_t PRIVATE LIBRARY_MANAGER_SEARCH_PATH=1)

cet_test(replace_all_test USE_BOOST_UNIT LIBRARIES PRIVATE cetlib::cetlib)
cet_test(regex_t USE_BOOST_UNIT LIBRARIES PRIVATE cetlib::cetlib DATAFILES regex.txt)
cet_test(regex_standalone_t SOURCE regex_t.cc DATAFILES regex.txt)

set_target_properties(regex_standalone_t PROPERTIES COMPILE_DEFINITIONS STANDALONE_TEST)

########################################################################
# Catch2 unit tests

cet_test(getenv_test USE_CATCH2_MAIN LIBRARIES PRIVATE Catch2::Catch2 cetlib::cetlib)
cet_test(os_libpath_t USE_CATCH2_MAIN LIBRARIES PRIVATE Catch2::Catch2)
cet_test(search_path_test USE_CATCH2_MAIN TEST_PROPERTIES ENVIRONMENT xyzzy=""
         LIBRARIES PRIVATE Catch2::Catch2 cetlib::cetlib)
cet_test(canonical_number_test USE_CATCH2_MAIN
         LIBRARIES PRIVATE Catch2::Catch2 cetlib::cetlib)
cet_test(crc32_test USE_CATCH2_MAIN
         SOURCE crc32_test.cc CRC32Calculator.cc
         TEST_ARGS -s
         LIBRARIES PRIVATE Catch2::Catch2 cetlib::cetlib)

# Use ParseAndAddCatchTests to generate a test for each test case.
cet_test(hypot_test USE_CATCH2_MAIN NO_AUTO LIBRARIES PRIVATE Catch2::Catch2)

include(ParseAndAddCatchTests)
set(AdditionalCatchParameters -s)
ParseAndAddCatchTests(hypot_test)
