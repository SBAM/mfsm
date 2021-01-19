namespace mfsm
{

  template <ValidStateMachineDefinition_c T>
  template <typename... Args>
  state_machine<T>::state_machine(Args&&... args) :
    T(std::forward<Args>(args)...),
    state_(reverse_get<initial_state>(states_tl{}))
  {
  }


  template <ValidStateMachineDefinition_c T>
  template <typename EVT>
  void state_machine<T>::process_event(EVT&& evt)
  {
    using evt_t = std::remove_cvref_t<EVT>;
    using flt_t = decltype(filter_by_event<evt_t>(rows_tl{}));
    static_assert(length(flt_t{}) > 0, "unhandled event");
    auto match_res = process_event_match(evt, flt_t{});
    if (!match_res)
      throw std::runtime_error("LOL");
  }


  template <ValidStateMachineDefinition_c T>
  template <typename EVT, Type_list_c TL>
  bool state_machine<T>::process_event_match(const EVT& evt, TL)
  {
    using row_t = decltype(front(TL{}));
    using start_t = typename row_t::start_t;
    using next_t = typename row_t::next_t;
    using action_t = typename row_t::action_t;
    using guard_t = typename row_t::guard_t;
    constexpr auto start_idx = reverse_get<start_t>(states_tl{});
    if (state_ == start_idx)
    {
      if constexpr (std::is_same_v<guard_t, none>)
      {
        action_t{}(evt, *this, start_t{}, next_t{});
        state_ = reverse_get<next_t>(states_tl{});
        return true;
      }
      else
        if (guard_t{}(evt, *this, start_t{}, next_t{}))
        {
          action_t{}(evt, *this, start_t{}, next_t{});
          state_ = reverse_get<next_t>(states_tl{});
          return true;
        }
    }
    if constexpr (length(TL{}) == 1)
      return false;
    else
    {
      using pf_tl = decltype(pop_front(TL{}));
      return process_event_match(evt, pf_tl{});
    }
  }


  template <ValidStateMachineDefinition_c T>
  inline std::size_t state_machine<T>::get_state() const
  {
    return state_;
  }

} // !namespace mfsm

