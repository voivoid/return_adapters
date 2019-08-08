#include "catch2/catch.hpp"

#include "return_adapters/out_retval_adapter.h"
#include "return_adapters/predicates.h"
#include "test_utils.h"

using namespace return_adapters;

TEST_CASE( "Check 'out_retval_optional_adapter' with 'check_retval_is_true' predicate", "out_retval_adapter" )
{
  constexpr auto adapted_div = out_retval::adapt<&ra_tests::divide, out_retval::out_retval_optional_adapter<check_retval_is_true>>();
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
