namespace mfsm
{

  template <Row_c... Rs>
  consteval auto make_unique_states_tl()
  {
    return unique(type_list<typename Rs::start_t...>{} +
                  type_list<typename Rs::next_t...>{});
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
