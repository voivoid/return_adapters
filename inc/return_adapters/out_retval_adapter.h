#pragma once

#include "boost/callable_traits/args.hpp"
#include "tuple_utils.h"

#include <optional>
#include <tuple>

#include <cassert>

namespace return_adapters
{

namespace out_retval
{

namespace details
{

  template <typename OutRet>
  struct OutRetArg;

  template <typename OutRet>
  struct OutRetArg<OutRet*>
  {
    auto* get() { return &val; }
    OutRet val;
  };

  template <typename OutRet>
  struct OutRetArg<OutRet&>
  {
    auto& get() { return val; }
    OutRet val;
  };

template <auto* adaptee_func, typename OutRet, typename ArgsBefore, typename ArgsAfter, typename OutRetValAdapter>
struct out_retval_adapter_impl;

template <auto* adaptee_func, typename OutRet, typename... ArgsBefore, typename... ArgsAfter, typename OutRetValAdapter>
struct out_retval_adapter_impl<adaptee_func, OutRet, std::tuple<ArgsBefore...>, std::tuple<ArgsAfter...>, OutRetValAdapter>
{
  static auto retval_adapted_func( ArgsBefore... argsBefore, ArgsAfter... argsAfter )
  {
    OutRetArg<OutRet> arg;
    auto f_result = adaptee_func(std::forward<ArgsBefore>(argsBefore)..., arg.get(), std::forward<ArgsAfter>(argsAfter)...);
    return OutRetValAdapter{}(std::move(f_result), arg.val);
  }
};

template <auto* adaptee_func, typename ArgsTuple, typename OutRetValAdapter, size_t out_argument_index>
struct adapter;

template <auto* adaptee_func, typename... Args, typename OutRetValAdapter, size_t out_argument_index>
struct adapter<adaptee_func, std::tuple<Args...>, OutRetValAdapter, out_argument_index>
{
  using OutRet     = typename std::tuple_element<out_argument_index, std::tuple<Args...>>::type;
  using ArgsBefore = typename take_until<out_argument_index, Args...>::ResultTuple;
  using ArgsAfter  = typename drop_until<out_argument_index, Args...>::ResultTuple;

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

constexpr size_t first_arg = 0;
constexpr size_t last_arg  = size_t( std::numeric_limits<size_t>::max() );

}  // namespace out_retval

template <auto* adaptee_func, typename OutRetValAdapter, size_t out_argument = out_retval::last_arg>
constexpr auto* turn_outarg_to_retval()
{
  using ArgsTuple = boost::callable_traits::args_t<decltype(adaptee_func)>;
  constexpr size_t args_num = std::tuple_size_v<ArgsTuple>;
  constexpr size_t out_argument_index = out_argument == out_retval::last_arg ? args_num - 1 : out_argument;

  return out_retval::details::adapter<adaptee_func, ArgsTuple, OutRetValAdapter, out_argument_index>::retval_adapted_func;
}

template <auto* adaptee_func, typename RetChecker, size_t out_argument = out_retval::last_arg>
constexpr auto* turn_outarg_to_optional_retval = turn_outarg_to_retval < adaptee_func, out_retval::to_optional< RetChecker >, out_argument >;

}  // namespace return_adapters
