#pragma once

#include "return_adapters/retval_adapter.h"

#include <memory>
#include <type_traits>

namespace return_adapters
{

namespace raii
{

struct to_unique_ptr
{
  template <typename T>
  auto operator()( T* retval ) const
  {
    return std::unique_ptr<std::remove_pointer_t<T>>( retval );
  }
};

template <typename Deleter>
struct to_unique_ptr_with_deleter_
{
  template <typename T>
  auto operator()( T* retval ) const
  {
    return std::unique_ptr<std::remove_pointer_t<T>, Deleter>( retval );
  }
};

template <auto* DeleteFunc>
struct to_unique_ptr_with_deleter_func_
{
  template <typename T>
  auto operator()( T* retval ) const
  {
    return std::unique_ptr<std::remove_pointer_t<T>, decltype( DeleteFunc )>( retval, DeleteFunc );
  }
};

template <auto* adaptee_func, typename RAIIadapter = to_unique_ptr>
constexpr auto* adapt = map_retval<adaptee_func, RAIIadapter>;

}  // namespace raii

}  // namespace return_adapters
