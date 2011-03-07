# ======================================================================
#
# Testing
#
# ======================================================================

include(CetTest)

# Identify libraries to be linked:
link_libraries( cetlib )

cet_test(bit_test)
cet_test(canonical_number_test)
cet_test(canonical_string_test)
cet_test(coded_exception_test)
cet_test(column_width_test USE_BOOST_UNIT)
cet_test(container_algs_test)
cet_test(cpu_timer_test NO_AUTO)
cet_test(exception_collector_test)
cet_test(exception_test)
cet_test(exempt_ptr_test)
cet_test(filesystem_test)
cet_test(getenv_test)
cet_test(inc-expand_test.sh PREBUILT DEPENDENCIES inc-expand)
cet_test(include_test)
cet_test(includer_test)
cet_test(maybe_ref_test USE_BOOST_UNIT)
cet_test(nybbler_test)
cet_test(registry_test)
cet_test(registry_via_id_test)
cet_test(registry_via_id_test_2 NO_AUTO) # for now -- see test's source
cet_test(rpad_test USE_BOOST_UNIT)
cet_test(search_path_test)
cet_test(search_path_test_2 NO_AUTO)
cet_test(search_path_test_2.sh PREBUILT
  DEPENDENCIES search_path_test_2
  )
cet_test(sha1_test)
cet_test(sha1_test_performance NO_AUTO
  SOURCES sha1_test_performance.cc sha1.cpp
  )
cet_test(sha1_test_2 SOURCES sha1_test_2.cc sha1.cpp)
cet_test(split_path_test)
cet_test(split_test)
cet_test(value_ptr_test)
cet_test(value_ptr_test_2)
cet_test(is_absolute_filepath_t USE_BOOST_UNIT)
cet_test(search_path_test_3 USE_BOOST_UNIT)
cet_test(zero_init_test USE_BOOST_UNIT)
