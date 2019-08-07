#include "catch2/catch.hpp"

#include "return_adapters/predicates.h"
#include "return_adapters/retval_adapter.h"
#include "test_utils.h"

using namespace return_adapters;
using namespace return_adapters::retval;

TEST_CASE( "Check retval adapter with 'check_retval_is_zero' predicate", "retval_adapter" )
{
  constexpr auto* return_int_adapted = adapt<&ra_tests::return_int, check_retval_is_zero>();
  REQUIRE( return_int_adapted );

  CHECK( return_int_adapted( 0 ) );
  CHECK( !return_int_adapted( -1 ) );
}
