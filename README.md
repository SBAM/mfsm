# mfsm
This header-only library is heavily inspired by [boost MSM](https://www.boost.org/doc/libs/1_75_0/libs/msm/doc/HTML/index.html) and mimics how it defines its _transition tables_.
Only a subset of its features are implemented.

## Overview of differences
* requires a c++20 compiler
* many compile-time checks using concepts
* removes 5 arguments limitation on state-machine constructor
* removes _transition table_ limitations due to boost MPL vector/map size
* states don't require any decoration
* accepts non-copyable events
* auto-detects if deferred events are used

## Dependencies
* cxx20 compiler such as [g++](https://gcc.gnu.org/) or [clang++](https://clang.llvm.org/)
* [cmake](https://cmake.org/)
* [boost](https://www.boost.org/) (optional, required for unit-tests)

> Fedora 33 setup
```shell
$ dnf install cmake gcc-c++ boost-devel
```

## Build and install
```console
user@mach:~$ git clone https://github.com/SBAM/mfsm.git
user@mach:~$ cd mfsm
user@mach:~$ ./configure.sh --prefix [...]
user@mach:~$ make -C build/Release tests install
```

## Use in a CMake project
* add `[...install path...]/lib(64)/cmake` to `CMAKE_PREFIX_PATH`
* add to _CMakeLists.txt_ `find_package(mfsm REQUIRED)`
* this exposes a target `mfsm::mfsm` that can be linked to a library

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
