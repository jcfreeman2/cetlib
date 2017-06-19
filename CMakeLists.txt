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

cmake_minimum_required(VERSION 3.0)  

# ----------------------------------------------------------------------
# Describe product:

project(cetlib VERSION 3.00.01 )

# cetmodules contains our cmake modules
find_package(cetmodules REQUIRED)

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


find_ups_product (cetmodules)
find_ups_boost()
#find_ups_product(sqlite)
find_ups_product(cetlib_except)

# SQLite
cet_find_library(SQLITE3 NAMES sqlite3_ups PATHS ENV SQLITE_LIB NO_DEFAULT_PATH)
# find openSSL 
# when building on macOS, use CommonCrypto
if(NOT APPLE)
  find_package(OpenSSL REQUIRED )
endif()

# ----------------------------------------------------------------------
# Describe source tree layout:

add_subdirectory( ${PROJECT_NAME} )  # source code
add_subdirectory( perllib )          # Modular plugin skeleton generator
add_subdirectory( ups )              # ups files

# ----------------------------------------------------------------------
# Packaging utility

#include(UseCPack)

#
# ======================================================================
