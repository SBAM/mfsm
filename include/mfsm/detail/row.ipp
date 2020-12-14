namespace mfsm
{

  template <Row_c... Rs>
  inline constexpr auto make_unique_states_tl()
  {
    return unique(type_list<Rs::start_t...>);
  }

} // !namespace mfsm
