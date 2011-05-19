# ======================================================================
#
# Build/install script
#
# ======================================================================

# ----------------------------------------------------------------------
# Build this project's library:

set( LIB_NAME ${PROJECT_NAME} )

file( GLOB LIB_SOURCES
      RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
      *.cc
      ../polarssl/*.cc
)
list( REMOVE_ITEM LIB_SOURCES "inc-expand.cc" )

link_libraries( ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY} )
add_library( ${LIB_NAME} SHARED ${LIB_SOURCES} )

# ----------------------------------------------------------------------
# Build this project's executables:

add_executable( inc-expand inc-expand.cc )
target_link_libraries( inc-expand ${LIB_NAME} )

# ----------------------------------------------------------------------
# Install this project's headers, sources, library, and executables:

install_headers()
install_source()
install( TARGETS ${LIB_NAME}
         DESTINATION ${flavorqual_dir}/lib )
install( TARGETS inc-expand
         DESTINATION ${flavorqual_dir}/bin )

#
# ======================================================================
