cmake_minimum_required(VERSION 3.25)

project(mfsm
  DESCRIPTION "MFP state-machine"
  LANGUAGES CXX)

message(STATUS " ==> CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")
message(STATUS " ==> CMAKE_CXX_FLAGS_DEBUG=${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS " ==> CMAKE_CXX_FLAGS_RELEASE=${CMAKE_CXX_FLAGS_RELEASE}")

# check link-time-optimisation support (-flto)
include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED
  OUTPUT IPO_MSG
  LANGUAGES CXX)
if (IPO_SUPPORTED)
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
endif()

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
