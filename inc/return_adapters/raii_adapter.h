#pragma once

#include "return_adapters/retval_adapter.h"

#include <memory>
#include <type_traits>

namespace return_adapters
{

template <template <typename... Args> class smart_ptr, typename... SmartPtrArgs>
struct to_smart_ptr
{
  template <typename T>
  auto operator()( T* const retval ) const
  {
    return smart_ptr<T, SmartPtrArgs...>( retval );
  }
};

namespace details
{
template <auto* DeleterFunction>
struct deleter_function
{
  template <typename T>
  void operator()( T* const ptr ) const
  {
    DeleterFunction( ptr );
  }
};

using to_unique_ptr = to_smart_ptr<std::unique_ptr>;
using to_shared_ptr = to_smart_ptr<std::shared_ptr>;

template <typename Deleter>
using to_unique_ptr_with_deleter = to_smart_ptr<std::unique_ptr, Deleter>;

template <auto* DeleterFunc>
using to_unique_ptr_with_f_deleter = to_unique_ptr_with_deleter<details::deleter_function<DeleterFunc>>;

}  // namespace details

template <auto* adaptee_func>
constexpr auto* to_unique_ptr = map_retval<adaptee_func, details::to_unique_ptr>;

template <auto* adaptee_func>
constexpr auto* to_shared_ptr = map_retval<adaptee_func, details::to_shared_ptr>;

template <auto* adaptee_func, typename deleter>
constexpr auto* to_unique_ptr_with_deleter = map_retval<adaptee_func, details::to_unique_ptr_with_deleter<deleter>>;

template <auto* adaptee_func, auto* deleter_func>
constexpr auto* to_unique_ptr_with_f_deleter = map_retval<adaptee_func, details::to_unique_ptr_with_f_deleter<deleter_func>>;

}  // namespace return_adapters
