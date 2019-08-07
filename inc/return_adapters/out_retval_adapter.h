#pragma once

#include <optional>
#include <tuple>

#include <cassert>

namespace return_adapters
{

namespace out_retval
{

namespace details
{

template <typename... Ts>
using tuple_cat_t = decltype( std::tuple_cat( std::declval<Ts>()... ) );

template <typename T, typename... Args>
struct remove_last_argument
{
  using ResultTuple = tuple_cat_t<std::tuple<T>, typename remove_last_argument<Args...>::ResultTuple>;
};

template <typename T>
struct remove_last_argument<T>
{
  using ResultTuple = std::tuple<>;
};

template <auto* adaptee_func, typename OutRet, typename ArgsTuple, typename OutRetValAdapter>
struct out_retval_adapter_impl;

template <auto* adaptee_func, typename OutRet, typename... ArgsTuple, typename OutRetValAdapter>
struct out_retval_adapter_impl<adaptee_func, OutRet, std::tuple<ArgsTuple...>, OutRetValAdapter>
{
  static auto retval_adapted_func( ArgsTuple... args )
  {
    using OutRetVal = std::remove_pointer_t<OutRet>;
    using OutRetOpt = std::optional<OutRetVal>;

    OutRetVal out_ret;
    auto res = adaptee_func( std::forward<ArgsTuple>( args )..., &out_ret );
    if ( res )
    {
      return OutRetOpt{ std::move( out_ret ) };
    }
    return OutRetOpt{};
  }
};

template <auto* adaptee_func, typename FuncType, typename OutRetValAdapter>
struct out_retval_adapter;

template <auto* adaptee_func, typename Ret, typename... Args, typename OutRetValAdapter>
struct out_retval_adapter<adaptee_func, Ret ( * )( Args... ), OutRetValAdapter>
{
  using OutRet    = typename std::tuple_element<sizeof...( Args ) - 1, std::tuple<Args...>>::type;
  using ArgsTuple = typename remove_last_argument<Args...>::ResultTuple;

  static constexpr auto retval_adapted_func =
      out_retval_adapter_impl<adaptee_func, OutRet, ArgsTuple, OutRetValAdapter>::retval_adapted_func;
};

}  // namespace details

template <auto* func, typename OutRetValAdapter>
constexpr auto* adapt()
{
  return details::out_retval_adapter<func, decltype( func ), OutRetValAdapter>::retval_adapted_func;
}

}  // namespace out_retval

}  // namespace return_adapters
