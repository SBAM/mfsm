#ifndef MFSM_DETAIL_TYPE_LIST_HPP_
# define MFSM_DETAIL_TYPE_LIST_HPP_

# include <concepts>
# include <type_traits>

namespace mfsm
{

  /// @brief helps manipulate types
  template <typename... Ts>
  struct type_list {};

  /// @brief Checks that given type is a type_list
  template <typename>
  struct is_type_list : std::false_type {};

  template <typename... Ts>
  struct is_type_list<type_list<Ts...>> : std::true_type {};

  template <typename T>
  constexpr bool is_type_list_v = is_type_list<T>::value;

  template <typename T>
  concept Type_list_c = is_type_list_v<T>;

  /// @return true if SEARCHED_T is present in type_list TL
  template <typename SEARCHED_T, Type_list_c TL>
  inline constexpr bool has_type();

  /// @brief shorthand to corresponding has_type result
  template <typename... Ts>
  inline constexpr bool has_type_v = has_type<Ts...>();

  /// @return front element of type_list
  template <typename HEAD, typename... TAIL>
  inline constexpr auto front(type_list<HEAD, TAIL...>);

  /// @return back element of type_list
  template <typename HEAD, typename... TAIL>
  inline constexpr auto back(type_list<HEAD, TAIL...>);

  /// @return given type_list minus last type
  template <typename HEAD, typename... TAIL>
  inline constexpr auto pop_back(type_list<HEAD, TAIL...>);

  /// @return concatenation of two types lists
  template <typename... Ts, typename... Us>
  inline constexpr auto operator+(type_list<Ts...>, type_list<Us...>);

  /// @return given type_list with duplicate types removed
  template <typename... Ts>
  inline constexpr auto unique(type_list<Ts...>);

} // !namespace mfsm

# include "type_list.ipp"

#endif // !MFSM_DETAIL_TYPE_LIST_HPP_
