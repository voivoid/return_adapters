#pragma once

#include "boost/callable_traits/args.hpp"

namespace return_adapters
{
namespace exception
{

namespace details
{
template <auto* adaptee_func, typename ArgsTuple, typename ExceptionTransformer>
struct adapter;

template <auto* adaptee_func, typename... Args, typename ExceptionTransformer>
struct adapter<adaptee_func, std::tuple<Args...>, ExceptionTransformer>
{
  static decltype( auto ) exception_mapped_func( Args... args )
  {
    try
    {
      return adaptee_func( std::forward<Args>( args )... );
    }
    catch ( ... )
    {
      ExceptionTransformer{}();
    }
  }
};
}  // namespace details
}  // namespace exception

template <auto* adaptee_func, typename ExceptionTransformer>
constexpr auto* map_exception = &exception::details::adapter<adaptee_func,
                                                             boost::callable_traits::args_t<decltype( adaptee_func )>,
                                                             ExceptionTransformer>::exception_mapped_func;

}  // namespace return_adapters