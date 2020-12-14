cmake_minimum_required(VERSION 3.14)

project(mfsm
  DESCRIPTION "MFP state-machine"
  LANGUAGES CXX)

message("-- cmake toolchain file = ${CMAKE_TOOLCHAIN_FILE}")

# select C++20 as standard
set(CMAKE_CXX_STANDARD 20)

# Searches for dependencies
# - Boost
find_package(Boost REQUIRED COMPONENTS
  unit_test_framework)

# Provide libray directory
add_subdirectory(include)

# Provide tests directory
enable_testing()
add_subdirectory(unit-tests)
