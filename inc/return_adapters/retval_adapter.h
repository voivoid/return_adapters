#pragma once

#include <utility>

namespace return_adapters
{

namespace details
{

template <auto* adaptee_func, typename FuncType, typename RetValAdapter>
struct retval_adapter;

template <auto* adaptee_func, typename Ret, typename... Args, typename RetValAdapter>
struct retval_adapter<adaptee_func, Ret ( * )( Args... ), RetValAdapter>
{
  static auto retval_adapted_func( Args... args )
  {
    return RetValAdapter()( adaptee_func( std::forward<Args>( args )... ) );
  }
};

}  // namespace details

template <auto* func, typename RetValAdapter>
constexpr auto* adapt_func_retval()
{
  return &details::retval_adapter<func, decltype( func ), RetValAdapter>::retval_adapted_func;
}

}
