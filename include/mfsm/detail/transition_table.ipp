namespace mfsm
{

  template <Row_c... Rs>
  template <typename SM>
  consteval bool transition_table<Rs...>::is_valid()
  {
    return ((Row_action_c<Rs, SM> && Row_guard_c<Rs, SM>) && ...);
  }

} // !namespace mfsm
