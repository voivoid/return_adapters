#pragma once

#include "boost/callable_traits/args.hpp"
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
template <auto* adaptee_func, typename ArgsTuple, typename AdapteeFuncName, typename RetValHandler>
struct adapter;

template <auto* adaptee_func, typename... Args, typename AdapteeFuncName, typename RetValHandler>
struct adapter<adaptee_func, std::tuple<Args...>, AdapteeFuncName, RetValHandler>
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

template <typename ExceptionMsgFormatter = generic_exception_formatter,
          typename Exception = std::runtime_error>
struct generic_exception_maker
{
  template <typename Result>
  Exception operator()(const char* const func_name, Result result) const
  {
    return Exception(ExceptionMsgFormatter{}(func_name, std::move(result)));
  }
};

template <typename RetValuePredicate, typename ExceptionMaker = generic_exception_maker<> >
struct generic_adapter_handler
{
  template <typename Result>
  Result operator()( const char* const func_name, Result result ) const
  {
    if ( !RetValuePredicate{}( result ) )
    {
      throw ExceptionMaker{}(func_name, std::move(result));
    }

    return result;
  }
};

template <auto* func, typename AdapteeFuncName, typename RetValHandler>
constexpr auto* adapt()
{
  return &details::adapter<func, boost::callable_traits::args_t<decltype( func )>, AdapteeFuncName, RetValHandler>::throwing_func;
}

}  // namespace throwing

}  // namespace return_adapters


#define RETURN_ADAPTERS_ADAPT_TO_THROWING( func, handler ) return_adapters::throwing::adapt<&func, typestring_is( #func ), handler>()
#define RETURN_ADAPTERS_ADAPT_TO_THROWING_GENERIC( func, retval_predicate )                                                                \
  return_adapters::throwing::adapt<&func, typestring_is( #func ), generic_adapter_handler<retval_predicate>>()
