#pragma once

#include "predicates.h"

#include "boost/callable_traits/args.hpp"

#include <optional>
#include <utility>
#include <tuple>

namespace return_adapters
{
namespace retval
{

namespace details
{
template <auto* adaptee_func, typename ArgsTuple, typename RetValAdapter>
struct adapter;

template <auto* adaptee_func, typename... Args, typename RetValAdapter>
struct adapter<adaptee_func, std::tuple<Args...>, RetValAdapter>
{
  static auto retval_adapted_func( Args... args )
  {
    return RetValAdapter()( adaptee_func( std::forward<Args>( args )... ) );
  }
};

}  // namespace details



template <typename Predicate, typename Transform = id>
struct to_optional
{
  template <typename T>
  auto operator()( T&& retval ) const
  {
    using AdaptedType = std::decay_t<decltype( Transform{}( std::forward<T>( retval ) ) )>;
    using Optional    = std::optional<AdaptedType>;
    if ( Predicate{}( retval ) )
    {
      return Optional{ Transform{}( std::forward<T>( retval ) ) };
    }

    return Optional{};
  }
};

template <auto* func, typename RetValAdapter>
constexpr auto* adapt()
{
  return &details::adapter<func, boost::callable_traits::args_t<decltype (func)>, RetValAdapter>::retval_adapted_func;
}

}  // namespace retval
}  // namespace return_adapters
