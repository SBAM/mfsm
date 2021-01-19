#define BOOST_TEST_MODULE mfsm

#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>

#include <mfsm/detail/state_machine.hpp>

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
    static_assert(mfsm::length(flt_t{}) == 0);
    static_assert(std::is_same_v<flt_t, mfsm::type_list<>>);
  }
}

BOOST_AUTO_TEST_CASE(action_invokable)
{
  static_assert(mfsm::Row_action_c<r1, int>);
  static_assert(mfsm::Row_action_c<r1, dummy>);
  static_assert(mfsm::Row_action_c<r2, int>);
  static_assert(mfsm::Row_action_c<r2, dummy>);
  static_assert(ttt::is_valid<int>());
}

BOOST_AUTO_TEST_SUITE_END()
