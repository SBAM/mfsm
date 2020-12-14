#ifndef MFSM_DETAIL_ROW_HPP_
# define MFSM_DETAIL_ROW_HPP_

# include "type_list.hpp"

namespace mfsm
{

  template <typename S, typename E, typename N, typename A, typename G>
  struct row
  {
    using start_t = S;
    using event_t = E;
    using next_t = N;
    using action_t = A;
    using guard_t = G;
  };

  template <typename... Ts>
  struct is_row : std::false_type {};

  template <typename... Ts>
  struct is_row<row<Ts...>> : std::true_type {};

  template <typename T>
  constexpr bool is_row_v = is_row<T>::value;

  template <typename T>
  concept Row_c = is_row_v<T>;

  template <Row_c... Rs>
  inline constexpr auto make_unique_states_tl();

} // !namespace mfsm

# include "row.ipp"

#endif // !MFSM_DETAIL_ROW_HPP_
