cmake_minimum_required(VERSION 3.14)

project(mfsm
  DESCRIPTION "MFP state-machine"
  LANGUAGES CXX)

message(STATUS "cmake toolchain file = ${CMAKE_TOOLCHAIN_FILE}")

# expose GNU standard installation directories
set(CMAKE_INSTALL_DEFAULT_DIRECTORY_PERMISSIONS
  OWNER_READ OWNER_WRITE OWNER_EXECUTE
  GROUP_READ GROUP_EXECUTE
  WORLD_READ WORLD_EXECUTE)
include(GNUInstallDirs OPTIONAL)
message(STATUS "install_full_includedir=${CMAKE_INSTALL_FULL_INCLUDEDIR}")

# select C++20 as standard
set(CMAKE_CXX_STANDARD 20)

# Searches for dependencies
# - Boost
find_package(Boost QUIET COMPONENTS
  unit_test_framework)

# Provides libray directory
add_subdirectory(include)

# Provides cmake mfsmConfig to install
add_subdirectory(cmake)

# Provides tests directory
if(Boost_UNIT_TEST_FRAMEWORK_FOUND)
  enable_testing()
  add_subdirectory(unit-tests)
else()
  message(WARNING "Boost unit-test-framework missing, skipping check target")
endif()
