add_library(mfsm INTERFACE)

target_include_directories(mfsm
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

target_compile_features(mfsm
  INTERFACE
    cxx_std_20)

target_compile_options(mfsm
  INTERFACE
    ${MFSM_CXX_FLAGS})

target_link_libraries(mfsm)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/mfsm
  DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
