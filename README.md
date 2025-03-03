# mfsm
This header-only library is inspired by [boost MSM](https://www.boost.org/doc/libs/1_87_0/libs/msm/doc/HTML/index.html) and copies how it defines its _transition tables_.
Only a subset of Boost.MSM features are implemented.

## Overview of differences
* requires a c++23 compiler
* many compile-time checks using concepts
* removes 5 arguments limitation on state-machine constructor
* removes _transition table_ limitations due to boost MPL vector/map size
* states don't require any decoration
* accepts non-copyable events
* auto-detects if deferred events are used

## Dependencies
* c++23 compiler such as [clang++](https://clang.llvm.org/) or [g++](https://gcc.gnu.org/)
* [boost](https://www.boost.org/) (required for unit-tests)
* [python](https://www.python.org/) (required for conan build)

## Build and install
```console
user@mach:~$ git clone https://github.com/SBAM/mfsm.git
user@mach:~$ cd mfsm
user@mach:~$ ./test.sh
user@mach:~$ conan upload -r <my_remote> mfsm/1.3
```

## Interface overview
```cpp
struct sm
{
  using transition_table_t = mfsm::transition_table<
    //        Start   Event Next        Action       Guard
    //       +-------+-----+-----------+------------+--------+
    mfsm::row<state1, evt2, state2,     action,      mfsm::none>,
    mfsm::row<state1, evt1, mfsm::none, mfsm::defer, mfsm::none>,
    mfsm::row<state2, evt1, state1,     action,      mfsm::none>,
    mfsm::row<state2, evt2, mfsm::none, mfsm::defer, mfsm::none>,
    mfsm::row<state2, evt3, state1,     action,      guard>,
    mfsm::row<state2, evt3, state2,     action,      not_guard>>;

  using initial_state = state1;

  template <typename EVT>
  void no_transition(EVT&&, std::size_t);
};

auto actual_sm = mfsm::state_machine<sm>{};

actual_sm.process_event(evt3{true});
```
Refer to unit-tests for more examples.

## Additional notes
* minimizes state-machine size, declares deferred queue only if needed
* low runtime complexity, candidate rows (match event) are deduced at compile time
