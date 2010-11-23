# ======================================================================
#  cetlib main build file
#
#  cd .../path/to/build/directory
#  source .../path/to/cetlib/ups/setup_for_development
#  cmake [-DCMAKE_INSTALL_PREFIX=/install/path] .../path/to/cetlib
#  make
#  make test
#  make install
#  make package
# ======================================================================


# ----------------------------------------------------------------------
# Describe build tool requirements:

cmake_minimum_required(VERSION 2.6)  # use cmake 2.6 or later

# ----------------------------------------------------------------------
# Describe product:

project(cetlib)
set(product cetlib)
set(version v0_1_5)
set(qualifier a1)

# cetbuildtools contains our cmake modules
SET ( CETBUILDTOOLS_VERSION $ENV{CETBUILDTOOLS_VERSION} )
IF (NOT CETBUILDTOOLS_VERSION)
    MESSAGE (FATAL_ERROR "ERROR: setup cetbuildtools to get the cmake modules")
ENDIF()
set(CMAKE_MODULE_PATH "${CMAKE_ROOT}/Modules")
set(CMAKE_MODULE_PATH $ENV{CETBUILDTOOLS_DIR}/Modules ${CMAKE_MODULE_PATH})
message(STATUS "module path is ${CMAKE_MODULE_PATH}" )

set(CMAKE_SKIP_RPATH) # don't embed path in shared lib's or executables

enable_testing()

# define some necessary modules
include(FindUpsPackage)
include(FindUpsBoost)
include(SetFlavorQual)
include(ParseUpsVersion)

#set package version from ups version
set_version_from_ups( ${version} )
#define flavorqual and flavorqual_dir
set_flavor_qual()

# ----------------------------------------------------------------------
# Describe build prerequisites:

set( GCC_VERSION $ENV{GCC_VERSION} )  # compiler
if( NOT GCC_VERSION )
  message( FATAL_ERROR
           "Can't locate GCC_VERSION, required to test ${PROJECT_NAME}"
         )
endif()
message( STATUS "Using gcc ${GCC_VERSION}" )

find_ups_boost( v1_42_0 filesystem regex )
find_ups_product( cpp0x v0_0_0 )

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
add_subdirectory( test )             # test code
add_subdirectory( ups )              # ups files

# ----------------------------------------------------------------------
# packaging utility
include(UseCPack)

#
# ======================================================================
