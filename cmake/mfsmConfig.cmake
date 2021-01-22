include(FindPackageHandleStandardArgs)

find_path(MFSM_INCLUDE_DIR
  NAMES mfsm/state_machine.hpp
  HINTS @CMAKE_INSTALL_FULL_INCLUDEDIR@)

mark_as_advanced(MFSM_INCLUDE_DIR)

find_package_handle_standard_args(mfsm
  REQUIRED_VARS
    MFSM_INCLUDE_DIR)

set(MFSM_INCLUDE_DIRS ${MFSM_INCLUDE_DIR})

if(NOT TARGET mfsm::mfsm)
  add_library(mfsm::mfsm INTERFACE IMPORTED)
  set_target_properties(mfsm::mfsm PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${MFSM_INCLUDE_DIR}")
endif()
