#define BOOST_TEST_MODULE mfsm

#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>

#include <mfsm/state_machine.hpp>

namespace but = boost::unit_test;

struct evt1 {};
struct evt2 {};
struct evt3
{
  bool g_;
  explicit evt3(bool g) : g_(g) {}
  evt3(evt3&&) = default;
  evt3& operator=(evt3&&) = default;
  evt3(const evt3&) = delete;
  evt3& operator=(const evt3&) = delete;
};

struct state1 {};
struct state2 {};

struct empty_transition
{
  template <typename... Args>
  void operator()(Args&&...) {}
};

struct sm_1
{
  sm_1(int i) : i_(i) {}
  sm_1(sm_1&& rhs) : i_(rhs.i_) {}
  using transition_table_t = mfsm::transition_table<
    mfsm::row<state1, evt2, state2, empty_transition, mfsm::none>,
    mfsm::row<state2, evt1, state1, empty_transition, mfsm::none>
    >;
  using initial_state = state1;
  int i_;
};



struct track
{
  template <typename SM>
  void operator()(evt2, SM& sm, state1, state2)
  {
    ++sm.tran_1_2_;
  }
  template <typename SM>
  void operator()(evt1, SM& sm, state2, state1)
  {
    ++sm.tran_2_1_;
  }
  template <typename EVT3, typename SM, typename N>
  void operator()(EVT3&& evt, SM& sm, state2, N)
  {
    std::cout << "evt3.g_=" << std::boolalpha << evt.g_ << std::endl;
    if (!evt.g_)
      ++sm.tran_2_2_;
  }
};

struct guard
{
  template <typename SM, typename S, typename N>
  bool operator()(const evt3& evt, SM&, S, N)
  {
    return evt.g_;
  }
};

struct not_guard
{
  template <typename SM, typename S, typename N>
  bool operator()(const evt3& evt, SM&, S, N)
  {
    return !evt.g_;
  }
};

struct sm_2
{
  using transition_table_t = mfsm::transition_table<
    mfsm::row<state1, evt2, state2, track, mfsm::none>,
    mfsm::row<state1, evt1, mfsm::none, mfsm::defer, mfsm::none>,
    mfsm::row<state2, evt1, state1, track, mfsm::none>,
    mfsm::row<state2, evt2, mfsm::none, mfsm::defer, mfsm::none>,
    mfsm::row<state2, evt3, state1, track, guard>,
    mfsm::row<state2, evt3, state2, track, not_guard>
    >;
  using initial_state = state1;
  template <typename EVT>
  void no_transition(EVT&&, std::size_t) { ++no_tran_; }
  int tran_1_2_;
  int tran_2_1_;
  int tran_2_2_;
  int no_tran_;
};


BOOST_AUTO_TEST_SUITE(state_machine_test_suite)

BOOST_AUTO_TEST_CASE(valid_definition)
{
  struct dummy {};
  static_assert(!mfsm::ValidStateMachineDefinition_c<dummy>);
  static_assert(mfsm::ValidStateMachineDefinition_c<sm_1>);
  static_assert(mfsm::ValidStateMachineDefinition_c<sm_2>);
}

BOOST_AUTO_TEST_CASE(state_machine_initialization)
{
  auto tmp = mfsm::state_machine<sm_1>(42);
  std::cout << "sizeof(state_machine<sm_1>)=" << sizeof(tmp) << std::endl;
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

BOOST_AUTO_TEST_CASE(process_event_defer_and_guard)
{
  mfsm::state_machine<sm_2> tmp{};
  std::cout << "sizeof(state_machine<sm_2>)=" << sizeof(tmp) << std::endl;
  auto check = [&](int a, int b, int c, int d)
    {
      BOOST_CHECK_EQUAL(tmp.tran_1_2_, a);
      BOOST_CHECK_EQUAL(tmp.tran_2_1_, b);
      BOOST_CHECK_EQUAL(tmp.tran_2_2_, c);
      BOOST_CHECK_EQUAL(tmp.no_tran_, d);
    };
  tmp.process_event(evt1{});
  check(0, 0, 0, 0);
  tmp.process_event(evt2{});
  check(1, 1, 0, 0);
  tmp.process_event(evt1{});
  check(1, 1, 0, 0);
  tmp.process_event(evt2{});
  check(2, 2, 0, 0);
  tmp.process_event(evt3{false});
  check(2, 2, 0, 1);
  tmp.process_event(evt2{});
  check(3, 2, 0, 1);
  tmp.process_event(evt3{false});
  check(3, 2, 1, 1);
  tmp.process_event(evt3{false});
  check(3, 2, 2, 1);
  tmp.process_event(evt3{true});
  check(3, 2, 2, 1);
  tmp.process_event(evt2{});
  check(4, 2, 2, 1);
  tmp.reset_state();
  BOOST_CHECK_EQUAL(tmp.get_state(), 0);
}

BOOST_AUTO_TEST_SUITE_END()
