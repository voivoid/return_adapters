#pragma once

#include <utility>
#include <optional>
#include <type_traits>

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
struct retval_adapter<adaptee_func, Ret ( * )( Args... ) throw (), RetValAdapter>
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
    const T& operator()( const T& t ) const
    {
        return t;
    }
};

template <typename Predicate, typename Adapter>
struct to_optional
{
    template<typename T>
    auto operator()( const T& retval ) const
    {
        using AdaptedType = std::decay_t<decltype(Adapter{}( retval ))>;
        using OPT = std::optional<AdaptedType>;
        if( Predicate{}( retval ) )
        {
            return OPT{ Adapter{}( retval ) };
        }

        return OPT{};
    }
};

template <auto* func, typename RetValAdapter>
constexpr auto* adapt()
{
  return &details::retval_adapter<func, decltype( func ), RetValAdapter>::retval_adapted_func;
}

}  // namespace retval
}  // namespace return_adapters
