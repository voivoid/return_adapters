#pragma once

#include "return_adapters/retval_adapter.h"

#include <memory>
#include <type_traits>

namespace return_adapters
{
namespace raii
{

struct adapter_to_unique_ptr
{
  template <typename T>
  auto operator()( T* retval ) const
  {
    return std::unique_ptr<std::remove_pointer_t<T>>( retval );
  }
};

template <typename Deleter>
struct adapter_to_custom_unique_ptr
{
  template <typename T>
  auto operator()( T* retval ) const
  {
    return std::unique_ptr<std::remove_pointer_t<T>, Deleter>( retval );
  }
};

template <auto* func, typename RaiiAdapter = adapter_to_unique_ptr>
constexpr auto* adapt()
{
  return &retval::details::adapter<func, decltype( func ), RaiiAdapter>::retval_adapted_func;
}


}  // namespace raii
}  // namespace return_adapters
