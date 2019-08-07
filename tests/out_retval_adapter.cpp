#include "catch2/catch.hpp"

#include "return_adapters/out_retval_adapter.h"
#include "test_utils.h"

using namespace return_adapters;
using namespace return_adapters::out_retval;

TEST_CASE( "Check out_retval adapter", "out_retval_adapter" )
{
  constexpr auto adapted_div = adapt<&ra_tests::divide>();
  REQUIRE( adapted_div );

  SECTION( "Valid function arguments" )
  {
    std::optional<int> result = adapted_div( 6, 2 );
    REQUIRE( result );
    CHECK( *result == 3 );
  }

  SECTION( "Invalid function arguments" )
  {
    std::optional<int> res = adapted_div( 6, 0 );
    CHECK( !res );
  }
}
