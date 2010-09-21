# ======================================================================
#  cetlib main build file
#
#  cd .../path/to/build/directory
#  cmake [-DCMAKE_INSTALL_PREFIX=/install/path] .../path/to/cetlib
#  make
#  make test
#  make install
# ======================================================================

cmake_minimum_required(VERSION 2.6)

# ----------------------------------------------------------------------
# describe product

project(cetlib)
set(version v0_0_0)

set(qualifier a1)
set(flavorqual sl5.x86_64.${qualifier})
set(flavorqual_dir ${version}/${flavorqual} )

set(CMAKE_SKIP_RPATH) # don't embed path in shared lib's or executables

enable_testing()

# ----------------------------------------------------------------------
# describe build requirements

set( GCC_VERSION $ENV{GCC_VERSION} )  # compiler
if( NOT GCC_VERSION )
  message(FATAL_ERROR "Can't locate GCC_VERSION, required for testing cetlib")
endif()

include_directories( "${PROJECT_SOURCE_DIR}" ) # project's headers
include_directories( $ENV{BOOST_INC} )         # boost's headers

# ----------------------------------------------------------------------
# describe source tree structure

add_subdirectory(cetlib) # source code
add_subdirectory(test)   # test code
add_subdirectory(ups)    # ups table and config files
