include(${CMAKE_CURRENT_LIST_DIR}/CommonToolchain.cmake)

list(APPEND MFSM_CXX_FLAGS
  -fconcepts-diagnostics-depth=2)
