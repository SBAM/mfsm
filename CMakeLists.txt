cmake_minimum_required(VERSION 3.25)

project(mfsm
  DESCRIPTION "MFP state-machine"
  LANGUAGES CXX)

include(GNUInstallDirs OPTIONAL)

# Searches for dependencies
# - Boost
if (NOT BUILD_TESTING STREQUAL OFF)
  find_package(Boost QUIET REQUIRED COMPONENTS
    unit_test_framework)
endif()

# Provides libray directory
add_subdirectory(include)

if (NOT BUILD_TESTING STREQUAL OFF)
  enable_testing()
  add_subdirectory(unit-tests)
endif()
