#pragma once

#include "typestring.hh"
#include <utility>

namespace return_adapters
{

namespace details
{
template <auto* adaptee_func, typename TypeStr, typename FuncType, typename RetValHandler>
struct throwing_adapter;

template <auto* adaptee_func, typename TypeStr, typename Ret, typename... Args, typename RetValHandler>
struct throwing_adapter<adaptee_func, Ret ( * )( Args... ), TypeStr, RetValHandler>
{
  static auto throwing_func( Args... args )
  {
    return RetValHandler()( TypeStr::data(), adaptee_func( std::forward<Args>( args )... ) );
  }
};

}  // namespace details

struct check_ret_val_is_not_zero
{
  template <typename T>
  bool operator()( const T val ) const
  {
    return val != 0;
  }
};

struct generic_exception_formatter
{
  template <typename Result>
  std::string operator()( const char* const adaptee_func_name, const Result result ) const
  {
    return std::string( adaptee_func_name ) + " failed; returned value was: " + std::to_string( result );
  }
};

template <typename RetValueChecker, typename ExceptionMsgFormatter = generic_exception_formatter, typename Exception = std::runtime_error>
struct generic_throwing_adapter_handler
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
constexpr auto* adapt_to_throwing_func()
{
  return &details::throwing_adapter<func, decltype( func ), TypeStr, RetValHandler>::throwing_func;
}

#define RETURN_ADAPTERS_ADAPT_TO_THROWING( func, handler ) return_adapters::adapt_to_throwing_func<&func, typestring_is( #func ), handler>()
#define RETURN_ADAPTERS_ADAPT_TO_THROWING_GENERIC( func, retval_checker )                                                                  \
  return_adapters::adapt_to_throwing_func<&func, typestring_is( #func ), generic_throwing_adapter_handler<retval_checker>>()

}  // namespace return_adapters
