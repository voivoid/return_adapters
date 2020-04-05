#include "catch2/catch.hpp"

#include "return_adapters/out_retval_adapter.h"
#include "return_adapters/predicates.h"
#include "test_utils.h"

using namespace return_adapters;

TEST_CASE( "Check 'out_retval_optional_adapter' with 'check_retval_is_true' predicate", "out_retval_adapter" )
{
  constexpr auto adapted_div = out_retval::adapt<&ra_tests::divide, out_retval::to_optional<check_retval_is_true>>();
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

  constexpr auto adapted_div_with_1st_out_arg =
      out_retval::adapt<&ra_tests::divide_with_1st_out_arg, out_retval::to_optional<check_retval_is_true>, out_retval::first>();
  REQUIRE( adapted_div_with_1st_out_arg );

  {
    std::optional<int> result = adapted_div_with_1st_out_arg( 6, 2 );
    CHECK( result );
    CHECK( *result == 3 );
  }

  {
    std::optional<int> result = adapted_div_with_1st_out_arg( 6, 0 );
    CHECK( !result );
  }
}
