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
  void operator()(const evt2&, SM&, const state1&, const state2&)
  {
    std::cout << "empty_transition state1 -> state2" << std::endl;
  }
  template <typename SM>
  void operator()(const evt1&, SM&, const state2&, const state1&)
  {
    std::cout << "empty_transition state2 -> state1" << std::endl;
  }
};

struct sm_1
{
  sm_1(std::int32_t i) : i_(i) {}
  sm_1(sm_1&& rhs) : i_(rhs.i_) {}
  using transition_table_t = mfsm::transition_table<
    mfsm::row<state1, evt2, state2, empty_transition, mfsm::none>,
    mfsm::row<state2, evt1, state1, empty_transition, mfsm::none>
    >;
  using initial_state = state1;
  std::int32_t i_;
};


struct sm_2
{
  using transition_table_t = mfsm::transition_table<
    mfsm::row<state1, evt2, state2, empty_transition, mfsm::none>,
    mfsm::row<state1, evt1, state1, mfsm::defer, mfsm::none>,
    mfsm::row<state2, evt1, state1, empty_transition, mfsm::none>,
    mfsm::row<state2, evt2, state2, mfsm::defer, mfsm::none>
    >;
  using initial_state = state1;
  std::int32_t i_;
};


BOOST_AUTO_TEST_SUITE(state_machine_test_suite)

BOOST_AUTO_TEST_CASE(valid_definition)
{
  static_assert(!mfsm::ValidStateMachineDefinition_c<dummy>);
  static_assert(mfsm::ValidStateMachineDefinition_c<sm_1>);
  static_assert(mfsm::ValidStateMachineDefinition_c<sm_2>);
}

BOOST_AUTO_TEST_CASE(state_machine_initialization)
{
  auto tmp = mfsm::state_machine<sm_1>(42);
  BOOST_CHECK_EQUAL(tmp.get_state(), 0);
  BOOST_CHECK_EQUAL(tmp.i_, 42);
  auto tmp2 = std::move(tmp);
  BOOST_CHECK_EQUAL(tmp2.get_state(), 0);
  BOOST_CHECK_EQUAL(tmp2.i_, 42);
}

BOOST_AUTO_TEST_CASE(process_event_no_guard)
{
  auto tmp = mfsm::state_machine<sm_1>(42);
  tmp.process_event(evt2{});
  tmp.process_event(evt1{});
  tmp.process_event(evt2{});
  tmp.process_event(evt1{});
}

BOOST_AUTO_TEST_CASE(process_event_defer)
{
  mfsm::state_machine<sm_2> tmp;
  tmp.process_event(evt1{});
  tmp.process_event(evt2{});
  tmp.process_event(evt1{});
  tmp.process_event(evt2{});
}

BOOST_AUTO_TEST_SUITE_END()
