#pragma once

#include "typestring.hh"

#include "fmt/format.h"

#include <utility>

#include <cstring>

namespace return_adapters
{

namespace throwing
{


namespace details
{
template <auto* adaptee_func, typename TypeStr, typename FuncType, typename RetValHandler>
struct adapter;

template <auto* adaptee_func, typename TypeStr, typename Ret, typename... Args, typename RetValHandler>
struct adapter<adaptee_func, Ret ( * )( Args... ), TypeStr, RetValHandler>
{
  static auto throwing_func( Args... args )
  {
    return RetValHandler()( TypeStr::data(), adaptee_func( std::forward<Args>( args )... ) );
  }
};

}  // namespace details

struct check_ret_val_is_not_zero
{
  template <typename RetVal>
  bool operator()( const RetVal ret_val ) const
  {
    return ret_val != 0;
  }
};

struct check_ptr_is_not_null
{
  template <typename T>
  bool operator()( const T* ptr ) const
  {
    return ptr != nullptr;
  }
};

struct generic_exception_formatter
{
  template <typename Result>
  std::string operator()( const char* const adaptee_func_name, const Result result ) const
  {
    return fmt::format( "{} failed; returned value was: {}", adaptee_func_name, result );
  }
};

struct errno_exception_formatter
{
  template <typename Result>
  std::string operator()( const char* const adaptee_func_name, const Result ) const
  {
    return fmt::format( "{} failed; returned value was: {}", adaptee_func_name, errno );
  }
};

struct errno_str_exception_formatter
{
  template <typename Result>
  std::string operator()( const char* const adaptee_func_name, const Result ) const
  {
    return fmt::format( "{} failed; returned value was: {}", adaptee_func_name, strerror( errno ) );
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

template <auto* func, typename TypeStr, typename RetValHandler>
constexpr auto* adapt()
{
  return &details::adapter<func, decltype( func ), TypeStr, RetValHandler>::throwing_func;
}

}  // namespace throwing

}  // namespace return_adapters


#define RETURN_ADAPTERS_ADAPT_TO_THROWING( func, handler ) return_adapters::throwing::adapt<&func, typestring_is( #func ), handler>()
#define RETURN_ADAPTERS_ADAPT_TO_THROWING_GENERIC( func, retval_checker )                                                                  \
  return_adapters::throwing::adapt<&func, typestring_is( #func ), generic_adapter_handler<retval_checker>>()
