#ifndef MFSM_DETAIL_DEFER_QUEUE_HPP_
# define MFSM_DETAIL_DEFER_QUEUE_HPP_

# include <queue>

# include "transition_table.hpp"

namespace mfsm
{

  /**
   * @todo support polymorphic_allocator ?
   */
  template <TransitionTable_c T, bool HAS_DEFER = T::has_defer_v>
  class defer_queue {};

  template <TransitionTable_c T>
  class defer_queue<T, true>
  {
  protected:
    /// Shorthand to events variant queue
    using queue_t = std::queue<typename T::events_var>;
    queue_t queue_; ///< deferred events storage
  };

} // !namespace mfsm

#endif // !MFSM_DETAIL_DEFER_QUEUE_HPP_
