message(STATUS "Loading CommonToolchain.cmake")

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb3"
  CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ggdb1 -DNDEBUG"
  CACHE STRING "" FORCE)

string(APPEND CMAKE_CXX_FLAGS_INIT
  " -Wall"
  " -Wextra"
  " -Werror"
  " -Wconversion"
  " -Wshadow"
  " -Wunreachable-code")
