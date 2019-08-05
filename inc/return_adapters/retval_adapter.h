#pragma once

#include <optional>
#include <type_traits>
#include <utility>

namespace return_adapters
{
namespace retval
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

template <auto* adaptee_func, typename Ret, typename... Args, typename RetValAdapter>
struct retval_adapter<adaptee_func, Ret ( * )( Args... ) throw(), RetValAdapter>
{
  static auto retval_adapted_func( Args... args )
  {
    return RetValAdapter()( adaptee_func( std::forward<Args>( args )... ) );
  }
};

}  // namespace details

struct adapter_id
{
  template <typename T>
  auto&& operator()( T&& t ) const
  {
    return std::forward<T>( t );
  }
};

template <typename Predicate, typename Adapter = adapter_id>
struct to_optional
{
  template <typename T>
  auto operator()( T&& retval ) const
  {
    using AdaptedType = std::decay_t<decltype( Adapter{}( std::forward<T>( retval ) ) )>;
    using Optional    = std::optional<AdaptedType>;
    if ( Predicate{}( retval ) )
    {
      return Optional{ Adapter{}( std::forward<T>( retval ) ) };
    }

    return Optional{};
  }
};

template <auto* func, typename RetValAdapter>
constexpr auto* adapt()
{
  return &details::retval_adapter<func, decltype( func ), RetValAdapter>::retval_adapted_func;
}

}  // namespace retval
}  // namespace return_adapters
