namespace mfsm
{

  template <Row_c... Rs>
  consteval bool has_defer()
  {
    return (std::is_same_v<typename Rs::action_t, defer> || ...);
  }


  template <Row_c... Rs>
  consteval auto make_unique_states_tl()
  {
    return unique(type_list<typename Rs::start_t...>{} +
                  type_list<typename Rs::next_t...>{});
  }


  template <Row_c... Rs>
  consteval auto make_unique_events_tl()
  {
    return unique(type_list<typename Rs::event_t...>{});
  }



  template <Row_c... Rs>
  consteval auto make_variant_events()
  {
    if constexpr (has_defer<Rs...>())
    {
      using ue_tl = decltype(make_unique_events_tl<Rs...>());
      constexpr auto sub = []<typename... Ts>(type_list<Ts...>)
        {
          return std::variant<std::monostate, Ts...>{};
        };
      return sub(ue_tl{});
    }
    else
      return std::variant<std::monostate>{};
  }


  template <typename EVT, Row_c... Rs>
  consteval auto filter_by_event(type_list<Rs...>)
  {
    constexpr auto filter = []<Row_c R>(R)
      {
        if constexpr (std::is_same_v<typename R::event_t, EVT>)
          return type_list<R>{};
        else
          return type_list<>{};
      };
    return (filter(Rs{}) + ... + type_list<>{});
  }

} // !namespace mfsm
