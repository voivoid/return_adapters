#pragma once

#include "boost/callable_traits/args.hpp"
#include "boost/callable_traits/has_varargs.hpp"
#include "typestring.hh"

#include <cstring>
#include <sstream>
#include <tuple>
#include <utility>

namespace return_adapters
{

namespace throwing
{

struct grab_all_args
{
};


namespace details
{
template <auto* adaptee_func, typename ArgsTuple, bool VarArgs, typename AdapteeFuncName, typename RetValHandler, typename ArgToHandler>
struct adapter;

template <auto* adaptee_func, typename... Args, typename AdapteeFuncName, typename RetValHandler>
struct adapter<adaptee_func, std::tuple<Args...>, false, AdapteeFuncName, RetValHandler, void>
{
  static decltype( auto ) throwing_func( Args... args )
  {
    decltype( auto ) ret = adaptee_func( std::forward<Args>( args )... );
    RetValHandler{}( AdapteeFuncName::data(), std::move( ret ) );

    return static_cast<decltype( ret )>( ret );
  }
};

template <auto* adaptee_func, typename... Args, typename AdapteeFuncName, typename RetValHandler, size_t... ArgToHandlerIndices>
struct adapter<adaptee_func,
               std::tuple<Args...>,
               false,
               AdapteeFuncName,
               RetValHandler,
               std::integer_sequence<size_t, ArgToHandlerIndices...>>
{
  static decltype( auto ) throwing_func( Args... args )
  {
    std::tuple<Args&...> tuple( args... );
    decltype( auto ) ret = adaptee_func( std::forward<Args>( args )... );

    RetValHandler{}( AdapteeFuncName::data(), std::move( ret ), std::get<ArgToHandlerIndices>( tuple )... );

    return static_cast<decltype( ret )>( ret );
  }
};

template <auto* adaptee_func, typename... Args, typename AdapteeFuncName, typename RetValHandler, typename ArgToHandler>
struct adapter<adaptee_func, std::tuple<Args...>, false, AdapteeFuncName, RetValHandler, ArgToHandler>
{
  static decltype( auto ) throwing_func( Args... args )
  {
    std::tuple<Args&...> tuple( args... );
    decltype( auto ) ret = adaptee_func( std::forward<Args>( args )... );
    RetValHandler{}( AdapteeFuncName::data(), std::move( ret ), std::get<ArgToHandler&>( tuple ) );

    return static_cast<decltype( ret )>( ret );
  }
};

template <auto* adaptee_func, typename... Args, typename AdapteeFuncName, typename RetValHandler>
struct adapter<adaptee_func, std::tuple<Args...>, false, AdapteeFuncName, RetValHandler, grab_all_args>
{
  static decltype( auto ) throwing_func( Args... args )
  {
    decltype( auto ) ret = adaptee_func( std::forward<Args>( args )... );
    RetValHandler{}( AdapteeFuncName::data(), std::move( ret ), args... );

    return static_cast<decltype( ret )>( ret );
  }
};

template <auto* adaptee_func, typename... Args, typename AdapteeFuncName, typename RetValHandler, typename ArgToHandler>
struct adapter<adaptee_func, std::tuple<Args...>, true, AdapteeFuncName, RetValHandler, ArgToHandler>
{
  struct impl
  {
    template <typename... VarArgs>
    auto operator()( Args... args, VarArgs... var_args ) const
    {
      return details::adapter<adaptee_func, std::tuple<Args..., VarArgs...>, false, AdapteeFuncName, RetValHandler, ArgToHandler>::
          throwing_func( std::forward<Args>( args )..., std::forward<VarArgs>( var_args )... );
    }
  };

  inline static const auto throwing_func = impl{};
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
  std::string operator()( const char* const adaptee_func_name, const Result& ) const
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

using generic_exception = std::runtime_error;

template <typename ExceptionMsgFormatter = generic_exception_formatter, typename Exception = generic_exception>
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
  template <typename Result, typename... Args>
  decltype( auto ) operator()( const char* const func_name, Result&& result, const Args&... args ) const
  {
    if ( !RetValPredicate{}( result, args... ) )
    {
      throw ExceptionFactory{}( func_name, std::move( result ) );
    }
  }
};

}  // namespace throwing

template <auto* adaptee_func, typename AdapteeFuncName, typename RetValHandler, typename ArgToHandler = void>
constexpr auto make_throwing = throwing::details::adapter<adaptee_func,
                                                          boost::callable_traits::args_t<decltype( adaptee_func )>,
                                                          boost::callable_traits::has_varargs_v<decltype( adaptee_func )>,
                                                          AdapteeFuncName,
                                                          RetValHandler,
                                                          ArgToHandler>::throwing_func;

}  // namespace return_adapters


#define RETURN_ADAPTERS_ADAPT_TO_THROWING( func, handler ) return_adapters::make_throwing<&func, typestring_is( #func ), handler>
#define RETURN_ADAPTERS_ADAPT_TO_THROWING_WITH_ARG( func, handler, arg )                                                                   \
  return_adapters::make_throwing<&func, typestring_is( #func ), handler, arg>
#define RETURN_ADAPTERS_ADAPT_TO_THROWING_WITH_ALL_ARGS( func, handler )                                                                   \
  return_adapters::make_throwing<&func, typestring_is( #func ), handler, throwing::grab_all_args>
#define RETURN_ADAPTERS_ADAPT_TO_THROWING_WITH_INDICES( func, handler, ... )                                                               \
  return_adapters::make_throwing<&func, typestring_is( #func ), handler, std::integer_sequence<size_t, __VA_ARGS__>>
#define RETURN_ADAPTERS_ADAPT_TO_THROWING_GENERIC( func, retval_predicate )                                                                \
  return_adapters::make_throwing<&func, typestring_is( #func ), return_adapters::throwing::generic_adapter_handler<retval_predicate>>
