#pragma once

#include <utility>

namespace return_adapters
{

template <auto expected>
struct check_retval_is_
{
  template <typename RetVal>
  bool operator()( const RetVal& retval ) const
  {
    return retval == expected;
  }
};

template <auto expected>
struct check_retval_is_not_
{
  template <typename RetVal>
  bool operator()( const RetVal& retval ) const
  {
    return !check_retval_is_<expected>{}( retval );
  }
};

struct id
{
  template <typename T>
  auto&& operator()( T&& t ) const
  {
    return std::forward<T>( t );
  }
};

using check_retval_is_zero      = check_retval_is_<0>;
using check_retval_is_true      = check_retval_is_<true>;
using check_retval_is_not_zero  = check_retval_is_not_<0>;
using check_ret_ptr_is_not_null = check_retval_is_not_<nullptr>;

}  // namespace return_adapters
