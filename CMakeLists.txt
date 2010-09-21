# ======================================================================
#
# Main script to drive product's build/test/install
#
# ----------------------------------------------------------------------
#
#   cd /path/to/build_dir
#   cmake [-DCMAKE_INSTALL_PREFIX=/path/to/install_dir] /path/to/this_dir
#   make
#   make test
#   make install
# ======================================================================


# ----------------------------------------------------------------------
# Describe build tool requirements:

cmake_minimum_required(VERSION 2.6)

# ----------------------------------------------------------------------
# Describe product:

project(cetlib)
set(version v0_0_0)

set(qualifier a1)
set(flavorqual sl5.x86_64.${qualifier})
set(flavorqual_dir ${version}/${flavorqual} )

set(CMAKE_SKIP_RPATH) # don't embed path in shared lib's or executables

enable_testing()

# ----------------------------------------------------------------------
# Describe build requirements:

set( GCC_VERSION $ENV{GCC_VERSION} )  # compiler
if( NOT GCC_VERSION )
  message( FATAL_ERROR
           "Can't locate GCC_VERSION, needed to build/test ${PROJECT_NAME}"
         )
endif()

include_directories( "${PROJECT_SOURCE_DIR}" ) # project's headers
include_directories( $ENV{BOOST_INC} )         # boost's headers

# ----------------------------------------------------------------------
# Describe source tree substructure:

add_subdirectory( ${PROJECT_NAME} ) # source code
add_subdirectory( test )            # test code
add_subdirectory( ups )             # ups files

#
# ======================================================================
