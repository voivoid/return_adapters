#pragma once

namespace ra_tests
{

enum class int_value
{
  zero,
  non_zero
};

enum class ptr_value
{
    null_ptr,
    non_null_ptr
};

inline int return_int( const int_value val_to_return )
{
  return val_to_return == int_value::zero ? 0 : -1;
}

struct Obj {};

inline Obj* return_ptr( ptr_value val_to_return )
{
  return val_to_return == ptr_value::null_ptr ? nullptr : reinterpret_cast<Obj*>(1);
}

}
