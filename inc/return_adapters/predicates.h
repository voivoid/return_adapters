#pragma once

#include <functional>
#include <utility>

namespace return_adapters
{

template <typename Check>
struct negate
{
  template <typename RetVal>
  bool operator()( RetVal retval ) const
  {
    return !Check{}( retval );
  }
};

template <template <typename> class Predicate, auto expected>
struct check_retval
{
  template <typename RetVal>
  bool operator()( const RetVal retval ) const
  {
    return Predicate<decltype( retval )>{}( retval, expected );
  }
};

template <auto expected>
using check_retval_is_ = check_retval<std::equal_to, expected>;

template <auto expected>
using check_retval_is_not_ = negate<check_retval_is_<expected>>;

template <auto expected>
using check_retval_is_less_ = check_retval<std::less, expected>;

struct id
{
  template <typename T>
  decltype( auto ) operator()( T&& t ) const
  {
    return std::forward<T>( t );
  }
};

using check_retval_is_zero      = check_retval_is_<0>;
using check_retval_is_nullptr   = check_retval_is_<nullptr>;
using check_retval_is_true      = check_retval_is_<true>;
using check_retval_is_not_zero  = check_retval_is_not_<0>;
using check_ret_ptr_is_not_null = check_retval_is_not_<nullptr>;
using check_retval_is_less_zero = check_retval_is_less_<0>;

}  // namespace return_adapters
