# ======================================================================
#  cetlib main build file
#
#  cd .../path/to/build/directory
#  cmake [-DCMAKE_INSTALL_PREFIX=/install/path] .../path/to/cetlib
#  make
#  make test
#  make install
# ======================================================================


# ----------------------------------------------------------------------
# Describe build tool requirements:

cmake_minimum_required(VERSION 2.6)  # use cmake 2.6 or later

# ----------------------------------------------------------------------
# Describe product:

project(cetlib)
set(version v0_1_0)

set(qualifier a1)
set(flavorqual sl5.x86_64.${qualifier})
set(flavorqual_dir ${version}/${flavorqual} )

set(CMAKE_SKIP_RPATH) # don't embed path in shared lib's or executables

enable_testing()

# ----------------------------------------------------------------------
# Describe build prerequisites:

set( GCC_VERSION $ENV{GCC_VERSION} )  # compiler
if( NOT GCC_VERSION )
  message( FATAL_ERROR
           "Can't locate GCC_VERSION, required to test ${PROJECT_NAME}"
         )
endif()
message( STATUS "Using gcc ${GCC_VERSION}" )

set( BOOST_VERS $ENV{BOOST_VERSION} )  # boost
if( NOT BOOST_VERS )
  message( FATAL_ERROR
           "Can't locate BOOST_VERSION, required to test ${PROJECT_NAME}"
         )
endif()
set( BOOST_STRING $ENV{SETUP_BOOST} )
string( REGEX REPLACE ".*(-q.*)" "\\1" BOOST_QUAL "${BOOST_STRING}" )
set( BOOST_ROOT $ENV{BOOST_DIR} )
set( BOOST_INCLUDEDIR $ENV{BOOST_INC} )
set( BOOST_LIBRARYDIR $ENV{BOOST_LIB} )
set( Boost_USE_MULTITHREADED ON )
find_package( Boost 1.42 COMPONENTS filesystem )
message( STATUS "Using boost ${BOOST_VERS} ${BOOST_QUAL}" )

set( CPP0X_VERSION $ENV{CPP0X_VERSION} )  # cpp0x
if( NOT CPP0X_VERSION )
  message( FATAL_ERROR
           "Can't locate CPP0X_VERSION, needed to build/test ${PROJECT_NAME}"
         )
endif()
set( CPP0X_STRING $ENV{SETUP_CPP0X} )
string( REGEX REPLACE ".*(-q.*)" "\\1" CPP0X_QUAL "${CPP0X_STRING}" )
message( STATUS "Using cpp0x ${CPP0X_VERSION} ${CPP0X_QUAL}" )

# ----------------------------------------------------------------------
# Describe include path:

include_directories( "${PROJECT_SOURCE_DIR}" )  # project's headers
include_directories( $ENV{BOOST_INC} )          # boost's headers
include_directories( $ENV{CPP0X_INC} )          # cpp0x's headers

# ----------------------------------------------------------------------
# Describe source tree layout:

add_subdirectory( ${PROJECT_NAME} )  # source code
add_subdirectory( test )             # test code
add_subdirectory( ups )              # ups files

# ----------------------------------------------------------------------
# Describe build tree layout:

set( LIBRARY_OUTPUT_PATH    ${PROJECT_BINARY_DIR}/lib )
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )

#
# ======================================================================
