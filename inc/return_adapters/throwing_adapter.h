#pragma once

#include "boost/callable_traits/args.hpp"
#include "typestring.hh"

#include <cstring>
#include <sstream>
#include <tuple>
#include <utility>

namespace return_adapters
{

namespace throwing
{


namespace details
{
template <auto* adaptee_func, typename ArgsTuple, typename AdapteeFuncName, typename RetValHandler, typename Arg>
struct adapter;

template <auto* adaptee_func, typename... Args, typename AdapteeFuncName, typename RetValHandler>
struct adapter<adaptee_func, std::tuple<Args...>, AdapteeFuncName, RetValHandler, void>
{
  static decltype( auto ) throwing_func( Args... args )
  {
    return RetValHandler()( AdapteeFuncName::data(), adaptee_func( std::forward<Args>( args )... ) );
  }
};

template <auto* adaptee_func, typename... Args, typename AdapteeFuncName, typename RetValHandler, size_t... ArgIndices>
struct adapter<adaptee_func, std::tuple<Args...>, AdapteeFuncName, RetValHandler, std::integer_sequence<size_t, ArgIndices...>>
{
  static decltype( auto ) throwing_func( Args... args )
  {
    std::tuple<Args...> tuple( std::forward<Args>( args )... );
    return RetValHandler()( AdapteeFuncName::data(), adaptee_func( std::forward<Args>( args )... ), std::get<ArgIndices>( tuple )... );
  }
};

template <auto* adaptee_func, typename... Args, typename AdapteeFuncName, typename RetValHandler, typename Arg>
struct adapter<adaptee_func, std::tuple<Args...>, AdapteeFuncName, RetValHandler, Arg>
{
  static decltype( auto ) throwing_func( Args... args )
  {
    std::tuple<Args...> tuple( std::forward<Args>( args )... );
    return RetValHandler()( AdapteeFuncName::data(), adaptee_func( std::forward<Args>( args )... ), std::get<Arg>( tuple ) );
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
  std::string operator()( const char* const adaptee_func_name, const Result& ) const
  {
    return details::format_error_message( adaptee_func_name, "errno message", details::get_errno_string() );
  }
};

template <typename ExceptionMsgFormatter = generic_exception_formatter, typename Exception = std::runtime_error>
struct generic_exception_factory
{
  template <typename Result>
  Exception operator()( const char* const func_name, Result result ) const
  {
    return Exception( ExceptionMsgFormatter{}( func_name, std::move( result ) ) );
  }
};

template <typename RetValPredicate, typename ExceptionFactory = generic_exception_factory<>>
struct generic_adapter_handler
{
  template <typename Result>
  decltype( auto ) operator()( const char* const func_name, Result&& result ) const
  {
    if ( !RetValPredicate{}( result ) )
    {
      throw ExceptionFactory{}( func_name, std::move( result ) );
    }

    return std::forward<Result>( result );
  }
};

}  // namespace throwing

template <auto* adaptee_func, typename AdapteeFuncName, typename RetValHandler, typename Arg = void>
constexpr auto* make_throwing =
    &throwing::details::
        adapter<adaptee_func, boost::callable_traits::args_t<decltype( adaptee_func )>, AdapteeFuncName, RetValHandler, Arg>::throwing_func;

}  // namespace return_adapters


#define RETURN_ADAPTERS_ADAPT_TO_THROWING( func, handler ) return_adapters::make_throwing<&func, typestring_is( #func ), handler>
#define RETURN_ADAPTERS_ADAPT_TO_THROWING_WITH_ARG( func, handler, arg )                                                                   \
  return_adapters::make_throwing<&func, typestring_is( #func ), handler, arg>
#define RETURN_ADAPTERS_ADAPT_TO_THROWING_WITH_INDICES( func, handler, ... )                                                               \
  return_adapters::make_throwing<&func, typestring_is( #func ), handler, std::integer_sequence<size_t, __VA_ARGS__>>
#define RETURN_ADAPTERS_ADAPT_TO_THROWING_GENERIC( func, retval_predicate )                                                                \
  return_adapters::make_throwing<&func, typestring_is( #func ), return_adapters::throwing::generic_adapter_handler<retval_predicate>>
