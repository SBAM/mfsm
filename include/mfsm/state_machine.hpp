#ifndef MFSM_STATE_MACHINE_HPP_
# define MFSM_STATE_MACHINE_HPP_

# include "detail/state_machine_back.hpp"

namespace mfsm
{

  /**
   * @tparam T Valid user-defined state machine
   * @brief This class encapsulates state_machine_back. It exposes various
   *        state_machine_back's methods.
   * @see state_machine_back::process_event
   *      state_machine_back::get_state
   *      state_machine_back::reset_state
   */
  template <ValidStateMachineDefinition_c T>
  struct state_machine : public state_machine_back<T>
  {
    using state_machine_back<T>::state_machine_back;
  };

} // !namespace mfsm

#endif // !MFSM_STATE_MACHINE_HPP_
