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

cmake_minimum_required(VERSION 3.12...3.14 FATAL_ERROR)

# ----------------------------------------------------------------------
# Describe product:

project(cetlib VERSION 3.07.02 LANGUAGES CXX)

# cetbuildtools contains our cmake modules
find_package(cetmodules 1.01.01 REQUIRED)

include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS VIGILANT
  WERROR
  NO_UNDEFINED
  EXTRA_FLAGS -pedantic
  EXTRA_CXX_FLAGS -Wnon-virtual-dtor -Wdelete-non-virtual-dtor
  )

cet_report_compiler_flags()

# ----------------------------------------------------------------------
# Describe build prerequisites:

find_package(Boost 1.50.0 REQUIRED COMPONENTS filesystem regex)
find_library(SQLITE3 NAMES sqlite3 REQUIRED)
find_package(cetlib_except 1.01.00 REQUIRED)
find_package(Catch2 2.3.0 REQUIRED)
find_package(hep_concurrency REQUIRED)

# find openSSL
# when building on macOS, use CommonCrypto
if(NOT APPLE)
  find_package(OpenSSL REQUIRED)
endif()

# ----------------------------------------------------------------------
# Describe source tree layout:

add_subdirectory( ${PROJECT_NAME} )  # source code
add_subdirectory( perllib )          # Modular plugin skeleton generator
add_subdirectory( cmakeConfig )              # cmakeConfig files
