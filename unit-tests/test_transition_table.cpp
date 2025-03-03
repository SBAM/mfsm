#define BOOST_TEST_MODULE mfsm

#include <boost/test/unit_test.hpp>

#include <mfsm/detail/transition_table.hpp>

namespace but = boost::unit_test;

struct evt1 {};
struct evt2 {};
struct state1 {};
struct state2 {};

struct dummy {};

struct empty_transition
{
  template <typename SM>
  void operator()(const evt2&, SM&, const state1&, const state2&) {}
  template <typename SM>
  void operator()(const evt1&, SM&, const state2&, const state1&) {}
};

using r1 = mfsm::row<state1, evt2, state2, empty_transition, mfsm::none>;
using r2 = mfsm::row<state2, evt1, state1, empty_transition, mfsm::none>;

using ttt = mfsm::transition_table<r1, r2>;


BOOST_AUTO_TEST_SUITE(transition_table_test_suite)

BOOST_AUTO_TEST_CASE(local_types)
{
  static_assert(std::is_same_v<ttt::rows_tl, mfsm::type_list<r1, r2>>);
  static_assert(mfsm::length(ttt::states_tl{}) == 2);
  static_assert(std::is_same_v<ttt::states_tl, mfsm::type_list<state1, state2>>);
  static_assert(mfsm::length(ttt::events_tl{}) == 2);
  static_assert(std::is_same_v<ttt::events_tl, mfsm::type_list<evt2, evt1>>);
}

BOOST_AUTO_TEST_CASE(type_filtering)
{
  {
    using flt_t = decltype(mfsm::filter_by_event<evt2>(ttt::rows_tl{}));
    static_assert(mfsm::length(flt_t{}) == 1);
    static_assert(std::is_same_v<flt_t, mfsm::type_list<r1>>);
  }
  {
    using flt_t = decltype(mfsm::filter_by_event<int>(ttt::rows_tl{}));
    static_assert(mfsm::length(flt_t{}) == 0uz);
    static_assert(std::is_same_v<flt_t, mfsm::type_list<>>);
  }
}

BOOST_AUTO_TEST_CASE(action_invokable)
{
  static_assert(mfsm::Row_action_c<r1, int>);
  static_assert(mfsm::Row_action_c<r1, dummy>);
  static_assert(mfsm::Row_action_c<r2, int>);
  static_assert(mfsm::Row_action_c<r2, dummy>);
}

BOOST_AUTO_TEST_CASE(defer_utilities)
{
  static_assert(!ttt::has_defer_v);
  static_assert(std::is_same_v<ttt::events_var, std::variant<std::monostate>>);
  // ttt2
  using r3 = mfsm::row<state2, evt2, state2, mfsm::defer, mfsm::none>;
  using ttt2 = mfsm::transition_table<r1, r2, r3>;
  static_assert(ttt2::has_defer_v);
  using res_var = std::variant<evt2, evt1>;
  static_assert(std::is_same_v<ttt2::events_var, res_var>);
  // ttt3
  using r4 = mfsm::row<state1, evt1, mfsm::none, mfsm::defer, mfsm::none>;
  using ttt3 = mfsm::transition_table<r1, r2, r3, r4>;
  static_assert(mfsm::length(ttt3::events_tl{}) == 2);
  static_assert(std::is_same_v<ttt3::events_tl, mfsm::type_list<evt2, evt1>>);
  static_assert(ttt3::has_defer_v);
  using res_var = std::variant<evt2, evt1>;
  static_assert(std::is_same_v<ttt2::events_var, res_var>);
}

BOOST_AUTO_TEST_SUITE_END()
