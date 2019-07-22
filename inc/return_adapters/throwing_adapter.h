#pragma once

#include <utility>

namespace details
{
template <auto* adaptee_func, typename FuncType, typename RetValHandler>
struct throwing_adapter;

template <auto* adaptee_func, typename Ret, typename... Args, typename RetValHandler>
struct throwing_adapter<adaptee_func, Ret ( * )( Args... ), RetValHandler>
{
  static auto throwing_func( Args... args )
  {
    return RetValHandler()( adaptee_func( std::forward<Args>( args )... ) );
  }
};

}  // namespace details

template <auto* func, typename RetValHandler>
constexpr auto* adapt_to_throwing_func()
{
    return &details::throwing_adapter<func, decltype( func ), RetValHandler>::throwing_func;
}
