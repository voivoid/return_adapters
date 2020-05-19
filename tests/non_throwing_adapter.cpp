#include "catch2/catch.hpp"

#include "return_adapters/non_throwing_adapter.h"
#include "test_utils.h"

#include <stdexcept>

using namespace return_adapters;

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
  constexpr auto* adapted = make_non_throwing<test_utils::decrement_unsigned>;
  REQUIRE( adapted );

  SECTION( "The adaptee function throws no exception" )
  {
    auto result = adapted( 10 );
    REQUIRE( result );
    CHECK( *result == 9 );
  }

  SECTION( "The adaptee function throws a std exception and it's caught" )
  {
    CHECK( !adapted( 0 ) );
  }
}

TEST_CASE( "Calling a non-throwing adapted void functions", "[non_throwing_adapter]" )
{
  constexpr auto* adapted_throwing_function = make_non_throwing<test_utils::throwing_function>;
  REQUIRE( adapted_throwing_function );

  SECTION( "The adaptee function throws no exception" )
  {
    auto result = adapted_throwing_function(test_utils::throw_mode::dont_throw );
    CHECK( result );
  }

  SECTION( "The adaptee function throws a std exception and it's caught" )
  {
    auto result = adapted_throwing_function(test_utils::throw_mode::throw_std_based );
    CHECK( !result );
  }

  SECTION( "The adaptee function throws a non-std exception and it's not caught" )
  {
    CHECK_THROWS_AS( adapted_throwing_function(test_utils::throw_mode::throw_non_std_based ), test_utils::non_std_based_exception );
  }
}

TEST_CASE( "Calling a non-throwing adapted functions with custom handler", "[non_throwing_adapter]" )
{
  constexpr auto* adapted_throwing_function = make_non_throwing<&test_utils::throwing_function, handle_any_exception_handler>;
  REQUIRE( adapted_throwing_function );

  CHECK_NOTHROW( adapted_throwing_function(test_utils::throw_mode::throw_std_based ) );
  CHECK_NOTHROW( adapted_throwing_function(test_utils::throw_mode::throw_non_std_based ) );
  CHECK_NOTHROW( adapted_throwing_function(test_utils::throw_mode::dont_throw ) );
}

}  // namespace
