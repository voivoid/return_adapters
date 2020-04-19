#include "catch2/catch.hpp"

#include "return_adapters/out_retval_adapter.h"
#include "return_adapters/predicates.h"
#include "test_utils.h"

using namespace return_adapters;

TEST_CASE( "Check 'out_retval_optional_adapter' with 'check_retval_is_true' predicate", "out_retval_adapter" )
{
  constexpr auto adapted_div = turn_outarg_to_optional_retval<&ra_tests::divide, check_retval_is_true>();
  REQUIRE( adapted_div );

  {
    std::optional<int> result = adapted_div( 6, 2 );
    CHECK( result );
    CHECK( *result == 3 );
  }

  {
    std::optional<int> result = adapted_div( 6, 0 );
    CHECK( !result );
  }

  // TODO: get rid of copy-paste by parametrizing the test

  {
    constexpr auto adapted_div =
        turn_outarg_to_optional_retval<&ra_tests::divide_with_1st_out_arg, check_retval_is_true, out_retval::first_arg>();
    REQUIRE( adapted_div );

    {
      std::optional<int> result = adapted_div( 6, 2 );
      CHECK( result );
      CHECK( *result == 3 );
    }

    {
      std::optional<int> result = adapted_div( 6, 0 );
      CHECK( !result );
    }
  }

  {
    constexpr auto adapted_div = turn_outarg_to_optional_retval<&ra_tests::divide_with_outref, check_retval_is_true>();
    REQUIRE( adapted_div );

    {
      std::optional<int> result = adapted_div( 6, 2 );
      CHECK( result );
      CHECK( *result == 3 );
    }

    {
      std::optional<int> result = adapted_div( 6, 0 );
      CHECK( !result );
    }
  }
}
