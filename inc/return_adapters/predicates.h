#pragma once

template <auto expected>
struct check_retval_is_
{
    template <typename RetVal>
    bool operator()( const RetVal retval ) const
    {
      return retval == expected;
    }
};

template <auto expected>
struct check_retval_is_not_
{
    template <typename RetVal>
    bool operator()( const RetVal retval ) const
    {
        return !check_retval_is_<expected>{}( retval );
    }
};

using check_retval_is_zero = check_retval_is_<0>;
using check_retval_is_not_zero = check_retval_is_not_<0>;
using check_ret_ptr_is_not_null = check_retval_is_not_<nullptr>;
