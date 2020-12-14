#ifndef MFSM_DETAIL_TRANSITION_TABLE_HPP_
# define MFSM_DETAIL_TRANSITION_TABLE_HPP_

# include "row.hpp"

namespace mfsm
{

  template <Row_c... Rs>
  struct transition_table
  {
    using rows_tl = type_list<Rs...>;
    using states_tl = decltype(make_unique_states_tl<Rs...>());
  };

} // !namespace mfsm

#endif // !MFSM_DETAIL_TRANSITION_TABLE_HPP_
