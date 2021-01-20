#ifndef MFSM_DETAIL_TRANSITION_TABLE_HPP_
# define MFSM_DETAIL_TRANSITION_TABLE_HPP_

# include <variant>

# include "row.hpp"

namespace mfsm
{

  template <Row_c... Rs>
  struct transition_table
  {
    using rows_tl = type_list<Rs...>;
    using states_tl = decltype(make_unique_states_tl<Rs...>());
    using events_tl = decltype(make_unique_events_tl<Rs...>());
    using events_var = decltype(make_variant_events<Rs...>());
    static constexpr bool has_defer_v = has_defer<Rs...>();

    /// @return true if all rows' action & guard are valid
    template <typename SM>
    static consteval bool is_valid();
  };


  template <typename... Ts>
  struct is_transition_table : std::false_type {};

  template <typename... Ts>
  struct is_transition_table<transition_table<Ts...>> : std::true_type {};

  template <typename T>
  constexpr bool is_transition_table_v = is_transition_table<T>::value;

  template <typename T>
  concept TransitionTable_c = is_transition_table_v<T>;

} // !namespace mfsm

# include "transition_table.ipp"

#endif // !MFSM_DETAIL_TRANSITION_TABLE_HPP_
