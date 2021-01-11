#ifndef MFSM_DETAIL_SPECIAL_STRUCTS_HPP_
# define MFSM_DETAIL_SPECIAL_STRUCTS_HPP_

namespace mfsm
{

  /**
   * @brief This structure is used as a substitute to a required concept and
   *        is expected to behave as a default, for example an empty guard.
   */
  struct none {};

} // !namespace mfsm

#endif // !MFSM_DETAIL_SPECIAL_STRUCTS_HPP_
