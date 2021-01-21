#ifndef MFSM_DETAIL_SPECIAL_STRUCTS_HPP_
# define MFSM_DETAIL_SPECIAL_STRUCTS_HPP_

namespace mfsm
{

  /// @brief Used as an empty guard that would always evaluate to true
  struct none {};

  /// @brief Used as an action that queues event for later processing
  struct defer {};

} // !namespace mfsm

#endif // !MFSM_DETAIL_SPECIAL_STRUCTS_HPP_
