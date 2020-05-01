#pragma once

#include "predicates.h"

#include "boost/callable_traits/args.hpp"

#include <optional>
#include <tuple>
#include <utility>

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
  static decltype(auto) retval_mapped_func( Args... args )
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

}  // namespace retval

template <auto* adaptee_func, typename RetValAdapter>
constexpr auto* map_retval =
    &retval::details::adapter<adaptee_func, boost::callable_traits::args_t<decltype( adaptee_func )>, RetValAdapter>::retval_mapped_func;

}  // namespace return_adapters
