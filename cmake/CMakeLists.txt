configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mfsmConfig.cmake
  ${PROJECT_BINARY_DIR}/mfsmConfig.cmake
  @ONLY)

install(FILES ${PROJECT_BINARY_DIR}/mfsmConfig.cmake
  DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}/cmake/mfsm)
