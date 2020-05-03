#pragma once

#include "boost/callable_traits/args.hpp"
#include "boost/callable_traits/return_type.hpp"

#include <optional>
#include <stdexcept>
#include <tuple>
#include <utility>

namespace return_adapters
{

namespace non_throwing
{

namespace details
{

template <auto* adaptee_func, typename Ret, typename ArgsTuple, template <auto*, typename> class ExceptionHandler>
struct adapter;

template <auto* adaptee_func, typename Ret, typename... Args, template <auto*, typename> class ExceptionHandler>
struct adapter<adaptee_func, Ret, std::tuple<Args...>, ExceptionHandler>
{
  static decltype( auto ) non_throwing_func( Args... args )
  {
    return ExceptionHandler<adaptee_func, Ret>::handle( std::forward<Args>( args )... );
  }
};

}  // namespace details

template <auto* adaptee_func, typename Ret, typename ExceptionToCatch = std::exception>
struct generic_exception_handler
{
  template <typename... Args>
  static std::optional<Ret> handle( Args&&... args )
  {
    try
    {
      return adaptee_func( std::forward<Args>( args )... );
    }
    catch ( const ExceptionToCatch& )
    {
      return {};
    }
  }
};

template <auto* adaptee_func, typename ExceptionToCatch>
struct generic_exception_handler<adaptee_func, void, ExceptionToCatch>
{
  template <typename... Args>
  static bool handle( Args&&... args )
  {
    try
    {
      adaptee_func( std::forward<Args>( args )... );
      return true;
    }
    catch ( const ExceptionToCatch& )
    {
      return false;
    }
  }
};

template <auto* adaptee_func, typename Ret>
using std_exception_handler = generic_exception_handler<adaptee_func, Ret, std::exception>;

}  // namespace non_throwing

template <auto* adaptee_func, template <auto*, typename> class Handler = non_throwing::std_exception_handler>
constexpr auto* make_non_throwing = &non_throwing::details::adapter<adaptee_func,
                                                                    boost::callable_traits::return_type_t<decltype( adaptee_func )>,
                                                                    boost::callable_traits::args_t<decltype( adaptee_func )>,
                                                                    Handler>::non_throwing_func;

}  // namespace return_adapters
