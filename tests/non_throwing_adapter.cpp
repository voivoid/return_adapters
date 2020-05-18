#include "catch2/catch.hpp"

#include "return_adapters/non_throwing_adapter.h"
#include "test_utils.h"

#include <stdexcept>

using namespace return_adapters;
using namespace test_utils;

namespace
{

template <auto* adaptee_func, typename Ret>
struct handle_any_exception_handler
{
  template <typename... Args>
  static void handle( Args&&... args )
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



TEST_CASE( "Calling non-throwing adapted non-void functions", "[non_throwing_adapter]" )
{
  constexpr auto* adapted_dec_if_positive = make_non_throwing<ra_tests::dec_if_positive>;
  REQUIRE( adapted_dec_if_positive );

  SECTION( "The adaptee function throws no exception" )
  {
    auto result = adapted_dec_if_positive( 10 );
    REQUIRE( result );
    CHECK( *result == 9 );
  }

  SECTION( "The adaptee function throws a std exception and it's caught" )
  {
    auto result = adapted_dec_if_positive( 0 );
    CHECK( !result );
  }
}

TEST_CASE( "Calling a non-throwing adapted void functions", "[non_throwing_adapter]" )
{
  constexpr auto* adapted_throwing_function = make_non_throwing<throwing_function>;
  REQUIRE( adapted_throwing_function );

  SECTION( "The adaptee function throws no exception" )
  {
    auto result = adapted_throwing_function( throw_mode::dont_throw );
    CHECK( result );
  }

  SECTION( "The adaptee function throws a std exception and it's caught" )
  {
    auto result = adapted_throwing_function( throw_mode::throw_std_based );
    CHECK( !result );
  }

  SECTION( "The adaptee function throws a non-std exception and it's not caught" )
  {
    CHECK_THROWS_AS( adapted_throwing_function( throw_mode::throw_non_std_based ), non_std_based_exception );
  }
}

TEST_CASE( "Calling a non-throwing adapted functions with custom handler", "[non_throwing_adapter]" )
{
  constexpr auto* adapted_throwing_function = make_non_throwing<&throwing_function, handle_any_exception_handler>;
  REQUIRE( adapted_throwing_function );

  CHECK_NOTHROW( adapted_throwing_function( throw_mode::throw_std_based ) );
  CHECK_NOTHROW( adapted_throwing_function( throw_mode::throw_non_std_based ) );
  CHECK_NOTHROW( adapted_throwing_function( throw_mode::dont_throw ) );
}

}  // namespace
