namespace mfsm
{

  template <typename SEARCHED_T, Type_list_c TL>
  inline constexpr bool has_type()
  {
    constexpr auto sub = []<typename... Ts>(type_list<Ts...>)
      {
        return (std::is_same_v<SEARCHED_T, Ts> || ...);
      };
    return sub(TL{});
  }


  template <typename HEAD, typename... TAIL>
  inline constexpr auto front(type_list<HEAD, TAIL...>)
  {
    return HEAD{};
  }


  template <typename HEAD, typename... TAIL>
  inline constexpr auto back(type_list<HEAD, TAIL...>)
  {
    if constexpr (sizeof...(TAIL) == 0)
      return HEAD{};
    else
      return back(type_list<TAIL...>{});
  }


  template <typename HEAD, typename... TAIL>
  inline constexpr auto pop_back(type_list<HEAD, TAIL...>)
  {
    if constexpr (sizeof...(TAIL) == 0)
      return type_list<>{};
    else
      return type_list<HEAD>{} + pop_back(type_list<TAIL...>{});
  }


  template <typename... Ts, typename... Us>
  inline constexpr auto operator+(type_list<Ts...>, type_list<Us...>)
  {
    return type_list<Ts..., Us...>{};
  }


  template <typename... Ts>
  inline constexpr auto unique(type_list<Ts...>)
  {
    if constexpr (sizeof...(Ts) <= 1)
      return type_list<Ts...>{};
    else
    {
      using pb_t = decltype(pop_back(type_list<Ts...>{}));
      using b_t = decltype(back(type_list<Ts...>{}));
      if constexpr (has_type<b_t, pb_t>())
        return unique(pb_t{});
      else
        return unique(pb_t{}) + type_list<b_t>{};
    }
  }

} // !namespace mfsm
