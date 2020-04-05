#pragma once

#include "tuple_utils.h"

#include "boost/callable_traits/args.hpp"

#include <optional>
#include <tuple>

#include <cassert>

namespace return_adapters
{

namespace out_retval
{

namespace details
{

template <auto* adaptee_func, typename OutRet, typename ArgsBefore, typename ArgsAfter, typename OutRetValAdapter>
struct out_retval_adapter_impl;

template <auto* adaptee_func, typename OutRet, typename... ArgsBefore, typename... ArgsAfter, typename OutRetValAdapter>
struct out_retval_adapter_impl<adaptee_func, OutRet, std::tuple<ArgsBefore...>, std::tuple<ArgsAfter...>, OutRetValAdapter>
{
  static auto retval_adapted_func(ArgsBefore... argsBefore, ArgsAfter... argsAfter)
  {
    using OutRetVal = std::remove_pointer_t<OutRet>;

    OutRetVal out_ret;
    return OutRetValAdapter{}(adaptee_func(std::forward<ArgsBefore>(argsBefore)..., &out_ret, std::forward<ArgsAfter>(argsAfter)...), out_ret );
  }
};

template <auto* adaptee_func, typename ArgsTuple, typename OutRetValAdapter, size_t out_argument_index>
struct adapter;

template <auto* adaptee_func, typename... Args, typename OutRetValAdapter, size_t out_argument_index>
struct adapter<adaptee_func, std::tuple<Args...>, OutRetValAdapter, out_argument_index>
{
  using OutRet    = typename std::tuple_element<out_argument_index, std::tuple<Args...>>::type;
  using ArgsBefore = typename take_until< out_argument_index, Args...>::ResultTuple;
  using ArgsAfter = typename drop_until< out_argument_index + 1, Args...>::ResultTuple;

  static constexpr auto retval_adapted_func =
      out_retval_adapter_impl<adaptee_func, OutRet, ArgsBefore, ArgsAfter, OutRetValAdapter>::retval_adapted_func;
};

}  // namespace details

template <typename RetChecker>
struct to_optional
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

constexpr size_t first = 0;
constexpr size_t last = size_t(std::numeric_limits<size_t>::max());

template <auto* func, typename OutRetValAdapter, size_t out_argument = last>
constexpr auto* adapt()
{
  using ArgsTuple = boost::callable_traits::args_t<decltype (func)>;
  constexpr size_t args_num = std::tuple_size_v<ArgsTuple>;
  constexpr size_t out_argument_index = out_argument == last ? args_num - 1 : out_argument;

  return details::adapter<func, ArgsTuple, OutRetValAdapter, out_argument_index>::retval_adapted_func;
}

}  // namespace out_retval

}  // namespace return_adapters
