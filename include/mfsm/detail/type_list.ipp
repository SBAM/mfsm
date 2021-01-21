namespace mfsm
{

  template <typename SEARCHED_T, Type_list_c TL>
  consteval bool has_type()
  {
    using s_t = std::remove_cvref_t<SEARCHED_T>;
    constexpr auto sub = []<typename... Ts>(type_list<Ts...>)
      {
        return (std::is_same_v<s_t, Ts> || ...);
      };
    return sub(TL{});
  }


  template <typename... Ts>
  consteval std::size_t length(type_list<Ts...>)
  {
    return sizeof...(Ts);
  }


  template <typename HEAD, typename... TAIL>
  consteval auto front(type_list<HEAD, TAIL...>)
  {
    return HEAD{};
  }


  template <typename HEAD, typename... TAIL>
  consteval auto pop_front(type_list<HEAD, TAIL...>)
  {
    return type_list<TAIL...>{};
  }


  template <typename HEAD, typename... TAIL>
  consteval auto back(type_list<HEAD, TAIL...>)
  {
    if constexpr (sizeof...(TAIL) == 0)
      return HEAD{};
    else
      return back(type_list<TAIL...>{});
  }


  template <typename HEAD, typename... TAIL>
  consteval auto pop_back(type_list<HEAD, TAIL...>)
  {
    if constexpr (sizeof...(TAIL) == 0)
      return type_list<>{};
    else
      return type_list<HEAD>{} + pop_back(type_list<TAIL...>{});
  }


  template <std::size_t N, typename... Ts>
  consteval auto get(type_list<Ts...>)
  {
    static_assert(sizeof...(Ts) > N);
    if constexpr (N == 0)
      return front(type_list<Ts...>{});
    else
      return get<N - 1>(pop_front(type_list<Ts...>{}));
  }


  template <typename SEARCHED_T, Type_list_c TL, std::size_t N>
  consteval std::size_t reverse_get(TL)
  {
    using s_t = std::remove_cvref_t<SEARCHED_T>;
    using f_t = decltype(front(TL{}));
    if constexpr (std::is_same_v<s_t, f_t>)
      return N;
    else
    {
      using pf_t = decltype(pop_front(TL{}));
      return reverse_get<s_t, pf_t, N + 1>(pf_t{});
    }
  }


  template <typename... Ts, typename... Us>
  consteval auto operator+(type_list<Ts...>, type_list<Us...>)
  {
    return type_list<Ts..., Us...>{};
  }


  template <typename... Ts>
  consteval auto unique(type_list<Ts...>)
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


  template <typename REM_T, typename... Ts>
  consteval auto remove(type_list<Ts...>)
  {
    if constexpr (sizeof...(Ts) == 0)
      return type_list<>{};
    else
    {
      constexpr auto sub_remove =
        []<typename HEAD, typename... TAIL>(type_list<HEAD, TAIL...>)
          {
            using r_t = std::remove_cvref_t<REM_T>;
            if constexpr (std::is_same_v<r_t, HEAD>)
              return remove<r_t>(type_list<TAIL...>{});
            else
              return type_list<HEAD>{} + remove<r_t>(type_list<TAIL...>{});
          };
      return sub_remove(type_list<Ts...>{});
    }
  }

} // !namespace mfsm
