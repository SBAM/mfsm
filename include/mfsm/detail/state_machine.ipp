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
    auto match_res = match_event<flt_t>(std::forward<EVT>(evt));
    if constexpr (transition_table_t::has_defer_v)
    {
      while (match_res && !dq_t::queue_.empty())
        match_res = process_event_deferred();
    }
  }


  template <ValidStateMachineDefinition_c T>
  template <Type_list_c TL, typename EVT>
  bool state_machine<T>::match_event(EVT&& evt)
  {
    using evt_t = std::remove_cvref_t<EVT>;
    using row_t = decltype(front(TL{}));
    using start_t = typename row_t::start_t;
    constexpr auto start_idx = reverse_get<start_t>(states_tl{});
    if (state_ == start_idx)
    {
      if constexpr (std::is_same_v<typename row_t::guard_t, none>)
        return invoke_or_defer_action<row_t>(std::forward<EVT>(evt));
      else
        if (invoke_guard(evt, row_t{}))
          return invoke_or_defer_action<row_t>(std::forward<EVT>(evt));
    }
    if constexpr (length(TL{}) == 1)
    {
      if constexpr (Has_no_transition_c<sm_t, evt_t>)
        T::no_transition(std::forward<EVT>(evt), state_);
      return false;
    }
    else
    {
      using pf_tl = decltype(pop_front(TL{}));
      return match_event<pf_tl>(evt);
    }
  }


  template <ValidStateMachineDefinition_c T>
  template <Row_c R, typename EVT>
  bool state_machine<T>::invoke_or_defer_action(EVT&& evt)
  {
    if constexpr (std::is_same_v<typename R::action_t, defer>)
      /// @note next state is ignored when event is deferred
      dq_t::queue_.push(std::forward<EVT>(evt));
    else
    {
      typename R::start_t s{};
      typename R::next_t n{};
      typename R::action_t a{};
      a(std::forward<EVT>(evt), *this, s, n);
      state_ = reverse_get<typename R::next_t>(states_tl{});
    }
    return !std::is_same_v<typename R::action_t, defer>;
  }


  template <ValidStateMachineDefinition_c T>
  template <Row_c R, typename EVT>
  bool state_machine<T>::invoke_guard(const EVT& evt)
  {
    typename R::start_t s{};
    typename R::next_t n{};
    typename R::guard_t g{};
    return g(evt, *this, s, n);
  }


  template <ValidStateMachineDefinition_c T>
  bool state_machine<T>::process_event_deferred()
  {
    auto result = std::visit
      ([this]<typename EVT>(EVT&& evt) -> bool
       {
         using evt_t = std::remove_cvref_t<EVT>;
         using flt_t = decltype(filter_by_event<evt_t>(rows_tl{}));
         return this->match_deferred_event<flt_t>(std::forward<EVT>(evt));
       }, dq_t::queue_.front());
    if (result)
      dq_t::queue_.pop();
    return result;
  }


  template <ValidStateMachineDefinition_c T>
  template <Type_list_c TL, typename EVT>
  bool state_machine<T>::match_deferred_event(EVT&& evt)
  {
    using row_t = decltype(front(TL{}));
    using start_t = typename row_t::start_t;
    constexpr auto start_idx = reverse_get<start_t>(states_tl{});
    if (state_ == start_idx)
    {
      if constexpr (std::is_same_v<typename row_t::guard_t, none>)
        return invoke_action<row_t>(std::forward<EVT>(evt));
      else
        if (invoke_guard(evt, row_t{}))
          return invoke_action<row_t>(std::forward<EVT>(evt));
    }
    if constexpr (length(TL{}) == 1)
      return false;
    else
    {
      using pf_tl = decltype(pop_front(TL{}));
      return match_deferred_event<pf_tl>(evt);
    }
  }


  template <ValidStateMachineDefinition_c T>
  template <Row_c R, typename EVT>
  bool state_machine<T>::invoke_action(EVT&& evt)
  {
    if constexpr (std::is_same_v<typename R::action_t, defer>)
      return false;
    else
    {
      typename R::start_t s{};
      typename R::next_t n{};
      typename R::action_t a{};
      a(std::forward<EVT>(evt), *this, s, n);
      state_ = reverse_get<typename R::next_t>(states_tl{});
      return true;
    }
  }


  template <ValidStateMachineDefinition_c T>
  inline std::size_t state_machine<T>::get_state() const
  {
    return state_;
  }

} // !namespace mfsm

