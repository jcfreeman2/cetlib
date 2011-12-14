# ======================================================================
#  cetlib main build file
#
#  cd .../path/to/build/directory
#  source .../path/to/cetlib/ups/setup_for_development
#  cmake [-DCMAKE_INSTALL_PREFIX=/install/path]
#        -Dqualifier:STRING=$CETPKG_QUAL
#        -DCMAKE_BUILD_TYPE=$CETPKG_TYPE
#        $CETPKG_SOURCE
#  make
#  make test
#  make install
#  make package
# ======================================================================


# ----------------------------------------------------------------------
# Describe build tool requirements:

cmake_minimum_required(VERSION 2.8)  # use cmake 2.8 or later

# ----------------------------------------------------------------------
# Describe product:

# cetbuildtools contains our cmake modules
set( CETBUILDTOOLS_VERSION $ENV{CETBUILDTOOLS_VERSION} )
if( NOT CETBUILDTOOLS_VERSION )
  message( FATAL_ERROR "ERROR: setup cetbuildtools to get the cmake modules" )
endif()
set( CMAKE_MODULE_PATH $ENV{CETBUILDTOOLS_DIR}/Modules ${CMAKE_MODULE_PATH} )

include(CetCMakeEnv)

if ( NOT qualifier )
    message (FATAL_ERROR "ERROR: please call cmake with -Dqualifier:STRING=qualifier")
endif ( NOT qualifier )
cet_cmake_env( cetlib v1_03_02 ${qualifier} )

cet_set_compiler_flags(DIAGS VIGILANT EXTRA_FLAGS -pedantic)
if ("${qualifier}" STREQUAL "e1")
  cet_add_compiler_flags(CXX -std=c++0x;-Wno-deprecated-declarations)
endif()
cet_report_compiler_flags()

# ----------------------------------------------------------------------
# Describe build prerequisites:

cet_check_gcc()

find_ups_product(cetbuildtools v1_15_00)
find_ups_boost( v1_48_0 filesystem regex unit_test_framework )
find_ups_product( cpp0x v1_03_02 )

# ----------------------------------------------------------------------
# Describe include path:

include_directories( "${PROJECT_SOURCE_DIR}" )  # project's headers

# ----------------------------------------------------------------------
# Describe build tree layout:

set( LIBRARY_OUTPUT_PATH    ${PROJECT_BINARY_DIR}/lib )
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )

# ----------------------------------------------------------------------
# Describe source tree layout:

add_subdirectory( ${PROJECT_NAME} )  # source code
add_subdirectory( polarssl )         # externally-imported code
add_subdirectory( test )             # test code
add_subdirectory( ups )              # ups files

# ----------------------------------------------------------------------
# Packaging utility:

include( UseCPack )

#
# ======================================================================
