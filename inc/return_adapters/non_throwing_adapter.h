#pragma once

#include <optional>
#include <stdexcept>
#include <utility>

namespace return_adapters
{

namespace non_throwing
{

namespace details
{
template <auto* adaptee_func, typename Ret, typename... Args>
struct generic_exception_handler
{
  std::optional<Ret> operator()( Args... args ) const
  {
    try
    {
      return adaptee_func( std::forward<Args>( args )... );
    }
    catch ( std::exception& )
    {
      return {};
    }
  }
};

template <auto* adaptee_func, typename... Args>
struct generic_exception_handler<adaptee_func, void, Args...>
{
  bool operator()( Args... args ) const
  {
    try
    {
      adaptee_func( std::forward<Args>( args )... );
      return true;
    }
    catch ( std::exception& )
    {
      return false;
    }
  }
};

template <auto* adaptee_func, typename FuncType, template <auto*, typename, typename...> class Handler>
struct adapter;

template <auto* adaptee_func, template <auto*, typename, typename...> class Handler, typename Ret, typename... Args>
struct adapter<adaptee_func, Ret ( * )( Args... ), Handler>
{
  static auto non_throwing_func( Args... args )
  {
    return Handler<adaptee_func, Ret, Args...>()( std::forward<Args>( args )... );
  }
};
}  // namespace details



template <auto* func, template <auto*, typename, typename...> class Handler = details::generic_exception_handler>
constexpr auto* adapt()
{
  return &details::adapter<func, decltype( func ), Handler>::non_throwing_func;
}

}  // namespace non_throwing

}  // namespace return_adapters
