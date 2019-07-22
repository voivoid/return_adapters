#include "catch2/catch.hpp"

#include "return_adapters/non_throwing_adapter.h"

#include <stdexcept>

namespace
{
int dec_or_throw_if_greater_than_10( const int x )
{
  if ( x > 10 )
  {
    throw std::runtime_error( "test exception" );
  }

  return x - 1;
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

void try_to_do_things( const throw_mode mode )
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

template <auto* adaptee_func, typename Ret, typename... Args>
struct handle_any_exception_handler
{
  void operator()( Args... args ) const
  {
    try
    {
      adaptee_func( std::forward<Args>( args )... );
    }
    catch ( ... )
    {
    }
  }
};

}  // namespace

TEST_CASE( "Calling non-throwing adapted non-void functions", "non_throwing_adapter" )
{
  constexpr auto* dec_if_less_than_11 = adapt_to_non_throwing_func<dec_or_throw_if_greater_than_10>();
  REQUIRE( dec_if_less_than_11 );

  SECTION( "The adaptee function throws no exception" )
  {
    auto result = dec_if_less_than_11( 10 );
    REQUIRE( result );
    CHECK( *result == 9 );
  }

  SECTION( "the adaptee function throws a std exception" )
  {
    auto result = dec_if_less_than_11( 11 );
    CHECK( !result );
  }
}

TEST_CASE( "Calling a non-throwing adapted void functions", "non_throwing_adapter" )
{
  constexpr auto* do_things_without_throwing = adapt_to_non_throwing_func<try_to_do_things>();
  REQUIRE( do_things_without_throwing );

  SECTION( "The adaptee function throws no exception" )
  {
    auto result = do_things_without_throwing( throw_mode::dont_throw_exception );
    CHECK( result );
  }

  SECTION( "The adaptee function throws a std exception" )
  {
    auto result = do_things_without_throwing( throw_mode::throw_std_exception );
    CHECK( !result );
  }

  SECTION( "The adaptee function throws a non-std exception" )
  {
    CHECK_THROWS_AS( do_things_without_throwing( throw_mode::throw_non_std_exception ), non_std_exception );
  }
}

TEST_CASE( "Calling a non-throwing adapted functions with custom handler", "non_throwing_adapter" )
{
  constexpr auto* do_things_without_throwing = adapt_to_non_throwing_func<try_to_do_things, handle_any_exception_handler>();
  REQUIRE( do_things_without_throwing );

  CHECK_NOTHROW( do_things_without_throwing( throw_mode::throw_std_exception ) );
  CHECK_NOTHROW( do_things_without_throwing( throw_mode::throw_non_std_exception ) );
  CHECK_NOTHROW( do_things_without_throwing( throw_mode::dont_throw_exception ) );
}
