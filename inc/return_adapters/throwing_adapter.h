#pragma once

#include "typestring.hh"

#include <cstring>
#include <sstream>
#include <utility>

namespace return_adapters
{

namespace throwing
{


namespace details
{
template <auto* adaptee_func, typename AdapteeFuncName, typename AdapteeFuncType, typename RetValHandler>
struct adapter;

template <auto* adaptee_func, typename AdapteeFuncName, typename Ret, typename... Args, typename RetValHandler>
struct adapter<adaptee_func, Ret ( * )( Args... ), AdapteeFuncName, RetValHandler>
{
  static auto throwing_func( Args... args )
  {
    return RetValHandler()( AdapteeFuncName::data(), adaptee_func( std::forward<Args>( args )... ) );
  }
};

inline std::string get_errno_string()
{
#ifdef _MSC_VER
  char buff[ 256 ];
  strerror_s( buff, errno );
  return buff;
#else
  return strerror( errno );
#endif
}

template <typename T>
std::string format_error_message( const char* const adaptee_func_name, const char* const value_description, const T& value )
{
  std::ostringstream s;
  s << adaptee_func_name << " failed; " << value_description << ": " << value;
  return s.str();
}

}  // namespace details

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

struct generic_exception_formatter
{
  template <typename Result>
  std::string operator()( const char* const adaptee_func_name, const Result result ) const
  {
    return details::format_error_message( adaptee_func_name, "returned value", result );
  }
};

struct errno_exception_formatter
{
  template <typename Result>
  std::string operator()( const char* const adaptee_func_name, const Result ) const
  {
    return details::format_error_message( adaptee_func_name, "errno", errno );
  }
};

struct errno_str_exception_formatter
{
  template <typename Result>
  std::string operator()( const char* const adaptee_func_name, const Result ) const
  {
    return details::format_error_message( adaptee_func_name, "errno message", details::get_errno_string() );
  }
};

template <typename RetValueChecker, typename ExceptionMsgFormatter = generic_exception_formatter, typename Exception = std::runtime_error>
struct generic_adapter_handler
{
  template <typename Result>
  void operator()( const char* const func_name, const Result result ) const
  {
    if ( RetValueChecker{}( result ) )
    {
      throw Exception( ExceptionMsgFormatter{}( func_name, result ) );
    }
  }
};

template <auto* adaptee_func, typename AdapteeFuncName, typename RetValHandler>
constexpr auto* adapt()
{
  return &details::adapter<adaptee_func, decltype( adaptee_func ), AdapteeFuncName, RetValHandler>::throwing_func;
}

}  // namespace throwing

}  // namespace return_adapters


#define RETURN_ADAPTERS_ADAPT_TO_THROWING( func, handler ) return_adapters::throwing::adapt<&func, typestring_is( #func ), handler>()
#define RETURN_ADAPTERS_ADAPT_TO_THROWING_GENERIC( func, retval_checker )                                                                  \
  return_adapters::throwing::adapt<&func, typestring_is( #func ), generic_adapter_handler<retval_checker>>()
