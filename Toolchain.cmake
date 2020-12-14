# override default cmake flags
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3")

# MFSM specific compilation flags
set(MFSM_CXX_FLAGS
  -Wall -Wextra -Werror
  -Wconversion
  -Wshadow)
