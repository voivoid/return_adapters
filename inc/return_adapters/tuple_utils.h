#pragma once

namespace return_adapters
{

  template <typename... Ts>
  using tuple_cat_t = decltype(std::tuple_cat(std::declval<Ts>()...));

  template <size_t n, typename T, typename... Args>
  struct take_until
  {
    using ResultTuple = tuple_cat_t<std::tuple<T>, typename take_until<n - 1, Args...>::ResultTuple>;
  };

  template <typename T, typename... Args>
  struct take_until<0, T, Args...>
  {
    using ResultTuple = std::tuple<>;
  };

  template <size_t n, typename... Args>
  struct drop_until;

  template <size_t n, typename T, typename... Args>
  struct drop_until<n, T, Args...>
  {
    using ResultTuple = typename drop_until<n - 1, Args...>::ResultTuple;
  };

  template <typename... Args>
  struct drop_until<0, Args...>
  {
    using ResultTuple = std::tuple<Args...>;
  };

}