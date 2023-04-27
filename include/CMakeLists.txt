add_library(mfsm INTERFACE)

target_include_directories(mfsm
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

target_link_libraries(mfsm)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/mfsm
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
