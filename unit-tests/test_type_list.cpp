#define BOOST_TEST_MODULE mfmat

#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>

#include <mfsm/detail/type_list.hpp>

namespace but = boost::unit_test;

struct s1 {};
struct s2 {};
struct s3 {};
struct s4 {};

BOOST_AUTO_TEST_SUITE(type_list_test_suite)

BOOST_AUTO_TEST_CASE(test_has_type)
{
  using tl_t = mfsm::type_list<s1, s3>;
  static_assert(mfsm::has_type_v<s1, tl_t>);
  static_assert(!mfsm::has_type_v<s2, tl_t>);
  static_assert(mfsm::has_type<s3, tl_t>());
  static_assert(!mfsm::has_type<s4, tl_t>());
}


BOOST_AUTO_TEST_CASE(test_length)
{
  using tl1_t = mfsm::type_list<>;
  static_assert(mfsm::length(tl1_t{}) == 0);
  using tl2_t = mfsm::type_list<s1>;
  static_assert(mfsm::length(tl2_t{}) == 1);
  using tl3_t = mfsm::type_list<s1, s2>;
  static_assert(mfsm::length(tl3_t{}) == 2);
  using tl4_t = mfsm::type_list<s1, s2, s3>;
  static_assert(mfsm::length(tl4_t{}) == 3);
}


BOOST_AUTO_TEST_CASE(test_front_back_pop_1)
{
  using tl_t = mfsm::type_list<s1>;
  using f_t = decltype(mfsm::front(tl_t{}));
  static_assert(std::is_same_v<f_t, s1>);
  using b_t = decltype(mfsm::back(tl_t{}));
  static_assert(std::is_same_v<b_t, s1>);
  using tl_pf_t = decltype(mfsm::pop_front(tl_t{}));
  static_assert(std::is_same_v<mfsm::type_list<>, tl_pf_t>);
  using tl_pb_t = decltype(mfsm::pop_back(tl_t{}));
  static_assert(std::is_same_v<mfsm::type_list<>, tl_pb_t>);
}


BOOST_AUTO_TEST_CASE(test_front_back_pop_2)
{
  using tl_t = mfsm::type_list<s1, s2, s3, s4>;
  using f_t = decltype(mfsm::front(tl_t{}));
  static_assert(std::is_same_v<f_t, s1>);
  using b_t = decltype(mfsm::back(tl_t{}));
  static_assert(std::is_same_v<b_t, s4>);
  using tl_pf_t = decltype(mfsm::pop_front(tl_t{}));
  static_assert(std::is_same_v<mfsm::type_list<s2, s3, s4>, tl_pf_t>);
  using tl_pb_t = decltype(mfsm::pop_back(tl_t{}));
  static_assert(std::is_same_v<mfsm::type_list<s1, s2, s3>, tl_pb_t>);
}


BOOST_AUTO_TEST_CASE(test_get)
{
  using tl_t = mfsm::type_list<s1, s2, s3, s4>;
  using get_0 = decltype(mfsm::get<0>(tl_t{}));
  static_assert(std::is_same_v<get_0, s1>);
  using get_1 = decltype(mfsm::get<1>(tl_t{}));
  static_assert(std::is_same_v<get_1, s2>);
  using get_2 = decltype(mfsm::get<2>(tl_t{}));
  static_assert(std::is_same_v<get_2, s3>);
  using get_3 = decltype(mfsm::get<3>(tl_t{}));
  static_assert(std::is_same_v<get_3, s4>);
}


BOOST_AUTO_TEST_CASE(test_reverse_get)
{
  using tl_t = mfsm::type_list<s1, s2, s3, s4, s1, s2, s3, s4>;
  static_assert(mfsm::reverse_get<s1>(tl_t{}) == 0);
  static_assert(mfsm::reverse_get<s2>(tl_t{}) == 1);
  static_assert(mfsm::reverse_get<s3>(tl_t{}) == 2);
  static_assert(mfsm::reverse_get<s4>(tl_t{}) == 3);
}


BOOST_AUTO_TEST_CASE(test_concatenate_1)
{
  using tl1 = mfsm::type_list<s1>;
  using tl2 = mfsm::type_list<s2>;
  using tl_res = mfsm::type_list<s1, s2>;
  using tl_cat = decltype(tl1{} + tl2{});
  static_assert(std::is_same_v<tl_res, tl_cat>);
}


BOOST_AUTO_TEST_CASE(test_concatenate_2)
{
  using tl1 = mfsm::type_list<s1, s2>;
  using tl2 = mfsm::type_list<s3, s4>;
  using tl_res = mfsm::type_list<s1, s2, s3, s4>;
  using tl_cat = decltype(tl1{} + tl2{});
  static_assert(std::is_same_v<tl_res, tl_cat>);
}


BOOST_AUTO_TEST_CASE(test_concatenate_3)
{
  using tl1 = mfsm::type_list<s1>;
  using tl2 = mfsm::type_list<s2>;
  using tl3 = mfsm::type_list<s3>;
  using tl4 = mfsm::type_list<s4>;
  using tl_res = mfsm::type_list<s1, s2, s3, s4>;
  using tl_cat = decltype(tl1{} + tl2{} + tl3{} + tl4{});
  static_assert(std::is_same_v<tl_res, tl_cat>);
}


BOOST_AUTO_TEST_CASE(test_unique_1)
{
  {
    using tl_res = mfsm::type_list<>;
    using tl_unique = decltype(mfsm::unique(tl_res{}));
    static_assert(std::is_same_v<tl_res, tl_unique>);
  }
  {
    using tl_res = mfsm::type_list<s1>;
    using tl_unique = decltype(mfsm::unique(tl_res{}));
    static_assert(std::is_same_v<tl_res, tl_unique>);
  }
  {
    using tl_res = mfsm::type_list<s1, s2>;
    using tl_unique = decltype(mfsm::unique(tl_res{}));
    static_assert(std::is_same_v<tl_res, tl_unique>);
  }
}


BOOST_AUTO_TEST_CASE(test_unique_2)
{
  {
    using tl = mfsm::type_list<s1, s1, s2, s1, s3, s1, s4, s1>;
    using tl_res = mfsm::type_list<s1, s2, s3, s4>;
    using tl_unique = decltype(mfsm::unique(tl{}));
    static_assert(std::is_same_v<tl_res, tl_unique>);
  }
  {
    using tl = mfsm::type_list<s1, s2, s3, s4, s1, s2, s3, s4>;
    using tl_res = mfsm::type_list<s1, s2, s3, s4>;
    using tl_unique = decltype(mfsm::unique(tl{}));
    static_assert(std::is_same_v<tl_res, tl_unique>);
  }
}

BOOST_AUTO_TEST_SUITE_END()
