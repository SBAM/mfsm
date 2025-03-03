message(STATUS "Loading ClangToolchain.cmake")

string(APPEND CMAKE_CXX_FLAGS_INIT
  " -Winconsistent-missing-override")
