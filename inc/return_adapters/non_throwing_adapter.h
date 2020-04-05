#pragma once

#include "boost/callable_traits/args.hpp"
#include "boost/callable_traits/return_type.hpp"

#include <optional>
#include <stdexcept>
#include <utility>
#include <tuple>

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

template <auto* adaptee_func, typename Ret, typename ArgsTuple, template <auto*, typename, typename...> class Handler>
struct adapter;

template <auto* adaptee_func, typename Ret, typename... Args, template <auto*, typename, typename...> class Handler>
struct adapter<adaptee_func, Ret, std::tuple<Args...>, Handler>
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
  using FuncType = decltype (func);
  return &details::adapter<func,
                           boost::callable_traits::return_type_t<FuncType>,
                           boost::callable_traits::args_t<FuncType>,
                           Handler>::non_throwing_func;
}

}  // namespace non_throwing

}  // namespace return_adapters
