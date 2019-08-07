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

    OutRetVal out_ret;
    return OutRetValAdapter{}( adaptee_func( std::forward<ArgsTuple>( args )..., &out_ret ), out_ret );
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

template <typename RetChecker>
struct out_retval_optional_adapter
{
  template <typename Ret, typename OutRet>
  std::optional<OutRet> operator()( const Ret& ret, OutRet& out_ret )
  {
    if ( RetChecker{}( ret ) )
    {
      return { std::move( out_ret ) };
    }
    return {};
  }
};

template <auto* func, typename OutRetValAdapter>
constexpr auto* adapt()
{
  return details::out_retval_adapter<func, decltype( func ), OutRetValAdapter>::retval_adapted_func;
}

}  // namespace out_retval

}  // namespace return_adapters
