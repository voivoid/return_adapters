#pragma once

#include <cassert>
#include <cstdarg>
#include <cstddef>
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

}  // namespace test_utils
}  // namespace return_adapters



namespace ra_tests
{

inline int ret_int_take_int( const int val )
{
  return val;
}
inline int ret_int_take_intref( int& val )
{
  return val;
}
inline int ret_int_take_intcref( const int& val )
{
  return val;
}
inline int& ret_intref_take_intref( int& val )
{
  return val;
}
inline const int& ret_intcref_take_intcref( const int& val )
{
  return val;
}

inline size_t dec_if_positive( const size_t val )
{
  if ( val > 0 )
  {
    return val - 1;
  }

  throw std::runtime_error( "underflow" );
}

inline int sum( int a, float b )
{
  return a + b;
}


enum class ptr_value_to_return
{
  null_ptr,
  non_null_ptr
};

struct Obj
{
};
inline Obj* return_ptr( ptr_value_to_return ptr_to_return )
{
  return ptr_to_return == ptr_value_to_return::null_ptr ? nullptr : reinterpret_cast<Obj*>( 1 );
}

class non_std_exception
{
};

enum class throw_mode
{
  throw_std_exception,
  throw_non_std_exception,
  dont_throw_exception
};

inline void throwing_function( const throw_mode mode )
{
  if ( mode == throw_mode::throw_std_exception )
  {
    throw std::runtime_error( "test exception" );
  }
  else if ( mode == throw_mode::throw_non_std_exception )
  {
    throw non_std_exception();
  }
}

inline int add_nums( const size_t count, ... )
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

}  // namespace ra_tests
