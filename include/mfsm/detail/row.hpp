#ifndef MFSM_DETAIL_ROW_HPP_
# define MFSM_DETAIL_ROW_HPP_

# include "type_list.hpp"
# include "special_structs.hpp"

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


  template <typename R, typename SM>
  concept Row_action_c = Row_c<R> &&
    (std::same_as<typename R::action_t, defer> ||
     requires (typename R::start_t start,
               typename R::event_t event,
               typename R::next_t next,
               typename R::action_t action,
               SM& sm)
     {
       action(event, sm, start, next);
     });

  template <typename R, typename SM>
  concept Row_guard_c = Row_c<R> &&
    (std::same_as<typename R::guard_t, none> ||
     requires (typename R::start_t start,
               const typename R::event_t& event,
               typename R::next_t next,
               typename R::guard_t guard,
               SM& sm)
     {
       { guard(event, sm, start, next) } -> std::same_as<bool>;
     });

  /// @return true if defer action is present in rows list Rs
  template <Row_c... Rs>
  consteval bool has_defer();

  template <Row_c... Rs>
  consteval auto make_unique_states_tl();

  template <Row_c... Rs>
  consteval auto make_unique_events_tl();

  template <Row_c... Rs>
  consteval auto make_variant_events();

  template <typename EVT, Row_c... Rs>
  consteval auto filter_by_event(type_list<Rs...>);

} // !namespace mfsm

# include "row.ipp"

#endif // !MFSM_DETAIL_ROW_HPP_
