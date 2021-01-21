#ifndef MFSM_DETAIL_STATE_MACHINE_HPP_
# define MFSM_DETAIL_STATE_MACHINE_HPP_

# include "defer_queue.hpp"

namespace mfsm
{

  template <typename T>
  concept ValidStateMachineDefinition_c =
    // state_machine needs to provide a valid transition_table
    TransitionTable_c<typename T::transition_table_t> &&
    // state_machine needs to provide an initial_state that exists in
    // transition_table
    has_type_v<typename T::initial_state,
               typename T::transition_table_t::states_tl>;


  template <typename SM, typename EVT>
  concept Has_no_transition_c = ValidStateMachineDefinition_c<SM> &&
    requires (SM& sm, EVT event, std::size_t state)
    {
      sm.no_transition(event, state);
    };


  /**
   * @todo Improve encapsulation and hide internal methods in a `back'
   *       state_machine class.
   */
  template <ValidStateMachineDefinition_c T>
  class state_machine :
    public T,
    protected defer_queue<typename T::transition_table_t>
  {
  public:
    using sm_t = state_machine<T>;
    using initial_state = typename T::initial_state;
    using transition_table_t = typename T::transition_table_t;
    using rows_tl = typename transition_table_t::rows_tl;
    using states_tl = typename transition_table_t::states_tl;
    using dq_t = defer_queue<transition_table_t>;

  public:
    template <typename... Args>
    state_machine(Args&&... args);

    template <typename EVT>
    void process_event(EVT&& evt);

    /// @return current state
    inline std::size_t get_state() const;

  private:
    /// @return true if processing was successful (queue can be popped)
    template <Type_list_c TL, typename EVT>
    bool match_event(EVT&& evt);

    /// @return true if evt was processed, false if evt was queued
    template <Row_c R, typename EVT>
    bool invoke_or_defer_action(EVT&& evt);

    /// @return transition's guard result
    template <Row_c R, typename EVT>
    bool invoke_guard(const EVT& evt);

    /// @return true if dequeuing was successful
    bool process_event_deferred();

    /// @return true if evt was processed
    template <Type_list_c TL, typename EVT>
    bool match_deferred_event(EVT&& evt);

    /// @return true if evt was processed
    template <Row_c R, typename EVT>
    bool invoke_action(EVT&& evt);

  private:
    std::size_t state_; ///< current state
    static_assert(transition_table_t::template is_valid<sm_t>(),
                  "invalid transition_table for this state_machine");
  };

} // !namespace mfsm

# include "state_machine.ipp"

#endif // !MFSM_DETAIL_STATE_MACHINE_HPP_
