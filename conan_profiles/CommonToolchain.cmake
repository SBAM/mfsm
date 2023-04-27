set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ggdb1 -DNDEBUG")

string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra -Wfatal-errors")
string(APPEND CMAKE_CXX_FLAGS " -Wconversion -Wshadow")
