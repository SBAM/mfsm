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
  consteval bool has_type();

  /// @brief shorthand to corresponding has_type result
  template <typename... Ts>
  constexpr bool has_type_v = has_type<Ts...>();

  /// @return given type_list's length
  template <typename... Ts>
  consteval std::size_t length(type_list<Ts...>);

  /// @return struct with member `type' aliasing front element of type_list
  template <typename HEAD, typename... TAIL>
  consteval auto front(type_list<HEAD, TAIL...>);

  /// @return given type_list minus first type
  template <typename HEAD, typename... TAIL>
  consteval auto pop_front(type_list<HEAD, TAIL...>);

  /// @return struct with member `type' aliasing back element of type_list
  template <typename HEAD, typename... TAIL>
  consteval auto back(type_list<HEAD, TAIL...>);

  /// @return given type_list minus last type
  template <typename HEAD, typename... TAIL>
  consteval auto pop_back(type_list<HEAD, TAIL...>);

  /// @return type_list's element at index N
  template <std::size_t N, typename... Ts>
  consteval auto get(type_list<Ts...>);

  /// @return SEARCHED_T's index from given type_list
  template <typename SEARCHED_T, Type_list_c TL, std::size_t N = 0>
  consteval std::size_t reverse_get(TL);

  /// @return concatenation of two types lists
  template <typename... Ts, typename... Us>
  consteval auto operator+(type_list<Ts...>, type_list<Us...>);

  /// @return given type_list with duplicate types removed
  template <typename... Ts>
  consteval auto unique(type_list<Ts...>);

  /// @return given type_list without given REM_T
  template <typename REM_T, typename... Ts>
  consteval auto remove(type_list<Ts...>);

} // !namespace mfsm

# include "type_list.ipp"

#endif // !MFSM_DETAIL_TYPE_LIST_HPP_
