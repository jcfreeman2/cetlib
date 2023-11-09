if (CETLIB)
  list(PREPEND CETLIB
    cetlib::sqlite
  )
  list(APPEND CETLIB
    cetlib::container_algorithms
    cetlib::parsed_program_options
    art_plugin_support::support_macros
  )
endif()