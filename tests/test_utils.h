#pragma once

#include <cassert>
#include <stdexcept>

namespace ra_tests
{

inline int return_int( const int val_to_return )
{
  return val_to_return;
}

inline int dec_if_positive( const int val )
{
  if ( val > 0 )
  {
    return val - 1;
  }

  throw std::runtime_error( "negative value" );
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

inline bool dec_if_positive_via_out_argument( int val, int* result )
{
  assert( result );

  if ( val > 0 )
  {
    *result = val - 1;
    return true;
  }

  return false;
}

}  // namespace ra_tests
