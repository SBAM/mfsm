#ifndef MFSM_DETAIL_STATE_MACHINE_BACK_HPP_
# define MFSM_DETAIL_STATE_MACHINE_BACK_HPP_

# include "defer_queue.hpp"

namespace mfsm
{

  /**
   * @brief This concept ensures a user-defined state machine has minimal
   *        requirements.
   */
  template <typename T>
  concept ValidStateMachineDefinition_c =
    // state_machine_back needs to provide a valid transition_table
    TransitionTable_c<typename T::transition_table_t> &&
    // state_machine_back needs to provide an initial_state that exists in
    // transition_table
    has_type_v<typename T::initial_state,
               typename T::transition_table_t::states_tl>;


  /**
   * @brief This concept checks if provided state machine has a no_transition
   *        method.
   */
  template <typename SM, typename EVT>
  concept Has_no_transition_c = ValidStateMachineDefinition_c<SM> &&
    requires (SM& sm, EVT&& event, std::size_t state)
    {
      sm.no_transition(std::forward<EVT>(event), state);
    };


  /**
   * @tparam T user-defined state machine
   * @brief This class manages events processing according to transition_table
   *        defined within user's state machine T.
   */
  template <ValidStateMachineDefinition_c T>
  class state_machine_back :
    public T,
    private defer_queue<typename T::transition_table_t>
  {
  public:
    using sm_t = state_machine_back<T>;
    using initial_state = typename T::initial_state;
    using transition_table_t = typename T::transition_table_t;
    using rows_tl = typename transition_table_t::rows_tl;
    using states_tl = typename transition_table_t::states_tl;
    using dq_t = defer_queue<transition_table_t>;

  public:
    /**
     * @brief Forwards args to user type T's constructor. Internal state is
     *        initialized using T::initial_state.
     */
    template <typename... Args>
    state_machine_back(Args&&... args);

    /// @param evt Event to process
    template <typename EVT>
    void process_event(EVT&& evt);

    /// @return current state
    inline std::size_t get_state() const;

    /// @brief Resets internal state to initial value and clears defer queue
    inline void reset_state();

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
    static_assert(transition_table_t::template is_valid<sm_t>());
  };

} // !namespace mfsm

# include "state_machine_back.ipp"

#endif // !MFSM_DETAIL_STATE_MACHINE_BACK_HPP_
