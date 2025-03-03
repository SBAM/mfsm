message(STATUS "Loading GNUToolchain.cmake")

string(APPEND CMAKE_CXX_FLAGS_INIT
  " -Wsuggest-override"
  " -fconcepts-diagnostics-depth=2")
