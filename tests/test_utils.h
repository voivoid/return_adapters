#pragma once

#include "return_adapters/predicates.h"

#include <cassert>
#include <cstdarg>
#include <cstddef>
#include <iosfwd>
#include <stdexcept>

namespace return_adapters
{
namespace test_utils
{
template <typename Ret, typename Arg>
Ret dummy( Arg )
{
  throw;
}

enum class error_code
{
  success = 0,
  unknown,
  permission_denied
};

std::ostream& operator<<( std::ostream& s, error_code c );

using check_is_success = check_retval_is_<test_utils::error_code::success>;

inline error_code read_file( error_code code_to_return )
{
  return code_to_return;
}

template <typename Ret, typename A1, typename A2>
Ret sum( A1 a1, A2 a2 )
{
  return a1 + a2;
}

inline int sum_varargs( const size_t count, ... )
{
  int result = 0;

  std::va_list args;
  va_start( args, count );
  for ( size_t i = 0; i < count; ++i )
  {
    result += va_arg( args, int );
  }
  va_end( args );

  return result;
}

enum class throw_mode
{
  throw_std_based,
  throw_non_std_based,
  dont_throw
};

class non_std_based_exception
{
};

inline void throwing_function( const throw_mode mode )
{
  if ( mode == throw_mode::throw_std_based )
  {
    throw std::runtime_error( "test exception" );
  }
  else if ( mode == throw_mode::throw_non_std_based )
  {
    throw non_std_based_exception();
  }
  else
  {
    assert( mode == throw_mode::dont_throw );
  }
}

template <typename Arg, typename OutArg>
inline bool divide( const Arg a, const Arg b, OutArg const result )
{
  if ( b != 0 )
  {
    if constexpr ( std::is_pointer_v<OutArg> )
    {
      assert( result );
      *result = a / b;
    }
    else
    {
      result = a / b;
    }

    return true;
  }

  return false;
}

inline size_t decrement_unsigned( const size_t val )
{
  if ( val > 0 )
  {
    return val - 1;
  }

  throw std::runtime_error( "underflow" );
}

}  // namespace test_utils
}  // namespace return_adapters
