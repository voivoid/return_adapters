#include "catch2/catch.hpp"

#include "return_adapters/non_throwing_adapter.h"
#include "test_utils.h"

#include <stdexcept>

using namespace return_adapters;

namespace
{

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
  constexpr auto* dec_if_positive = non_throwing::adapt<ra_tests::dec_if_positive>();
  REQUIRE( dec_if_positive );

  SECTION( "The adaptee function throws no exception" )
  {
    auto result = dec_if_positive( 10 );
    REQUIRE( result );
    CHECK( *result == 9 );
  }

  SECTION( "the adaptee function throws a std exception" )
  {
    auto result = dec_if_positive( 0 );
    CHECK( !result );
  }
}

TEST_CASE( "Calling a non-throwing adapted void functions", "non_throwing_adapter" )
{
  constexpr auto* almost_non_throwing_function = non_throwing::adapt<ra_tests::throwing_function>();
  REQUIRE( almost_non_throwing_function );

  SECTION( "The adaptee function throws no exception" )
  {
    auto result = almost_non_throwing_function( ra_tests::throw_mode::dont_throw_exception );
    CHECK( result );
  }

  SECTION( "The adaptee function throws a std exception" )
  {
    auto result = almost_non_throwing_function( ra_tests::throw_mode::throw_std_exception );
    CHECK( !result );
  }

  SECTION( "The adaptee function throws a non-std exception" )
  {
    CHECK_THROWS_AS( almost_non_throwing_function( ra_tests::throw_mode::throw_non_std_exception ), ra_tests::non_std_exception );
  }
}

TEST_CASE( "Calling a non-throwing adapted functions with custom handler", "non_throwing_adapter" )
{
  constexpr auto* non_throwing_function = non_throwing::adapt<ra_tests::throwing_function, handle_any_exception_handler>();
  REQUIRE( non_throwing_function );

  CHECK_NOTHROW( non_throwing_function( ra_tests::throw_mode::throw_std_exception ) );
  CHECK_NOTHROW( non_throwing_function( ra_tests::throw_mode::throw_non_std_exception ) );
  CHECK_NOTHROW( non_throwing_function( ra_tests::throw_mode::dont_throw_exception ) );
}
