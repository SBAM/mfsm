#ifndef MFSM_DETAIL_STATE_MACHINE_HPP_
# define MFSM_DETAIL_STATE_MACHINE_HPP_

# include "transition_table.hpp"

namespace mfsm
{

  template<typename T>
  concept ValidStateMachineDefinition_c =
    // state_machine needs to provide a valid transition_table
    TransitionTable_c<typename T::transition_table_t> &&
    // state_machine needs to provide an initial_state that exists in
    // transition_table
    has_type_v<typename T::initial_state,
               typename T::transition_table_t::states_tl>;


  template <ValidStateMachineDefinition_c T>
  class state_machine : public T
  {
  public:
    using sm_t = state_machine<T>;
    using initial_state = typename T::initial_state;
    using transition_table_t = typename T::transition_table_t;
    using rows_tl = typename transition_table_t::rows_tl;
    using states_tl = typename transition_table_t::states_tl;

  public:
    template <typename... Args>
    state_machine(Args&&... args);

    template <typename EVT>
    void process_event(EVT&& evt);

    /// @return current state
    inline std::size_t get_state() const;

  private:
    template <typename EVT, Type_list_c TL>
    bool process_event_match(const EVT& evt, TL);

  private:
    std::size_t state_; ///< current state
    static_assert(transition_table_t::template is_valid<sm_t>(),
                  "invalid transition_table for this state_machine");
  };

} // !namespace mfsm

# include "state_machine.ipp"

#endif // !MFSM_DETAIL_STATE_MACHINE_HPP_
