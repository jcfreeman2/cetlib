# ======================================================================
#  cetlib main build file
#
#  cd .../path/to/build/directory
#  source .../path/to/cetlib/ups/setup_for_development
#  cmake [-DCMAKE_INSTALL_PREFIX=/install/path]
#        -DCMAKE_BUILD_TYPE=$CETPKG_TYPE
#        $CETPKG_SOURCE
#  make
#  make test
#  make install
#  make package (builds distribution tarfile)
# ======================================================================


# ----------------------------------------------------------------------
# Describe build tool requirements:

cmake_minimum_required(VERSION 2.8)  # use cmake 2.8 or later

# ----------------------------------------------------------------------
# Describe product:

project(cetlib)

# cetbuildtools contains our cmake modules
set( CETBUILDTOOLS_VERSION $ENV{CETBUILDTOOLS_VERSION} )
if( NOT CETBUILDTOOLS_VERSION )
  message( FATAL_ERROR "ERROR: setup cetbuildtools to get the cmake modules" )
endif()
set( CMAKE_MODULE_PATH $ENV{CETBUILDTOOLS_DIR}/Modules ${CMAKE_MODULE_PATH} )

include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS VIGILANT WERROR EXTRA_FLAGS -pedantic)
if ("${qualifier}" STREQUAL "e1")
  cet_add_compiler_flags(CXX -std=c++0x;-Wno-deprecated-declarations)
endif()
cet_report_compiler_flags()

# ----------------------------------------------------------------------
# Describe build prerequisites:

cet_check_gcc()

find_ups_product(cetbuildtools v1_17_00)
find_ups_boost( v1_48_0 filesystem regex unit_test_framework )
find_ups_product( cpp0x v1_03_02 )

# ----------------------------------------------------------------------
# Describe source tree layout:

add_subdirectory( ${PROJECT_NAME} )  # source code
add_subdirectory( polarssl )         # externally-imported code
add_subdirectory( test )             # test code
add_subdirectory( ups )              # ups files

# ----------------------------------------------------------------------
# Packaging utility

include(UseCPack)

#
# ======================================================================
