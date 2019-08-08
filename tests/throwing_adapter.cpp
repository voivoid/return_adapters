#include "catch2/catch.hpp"

#include "return_adapters/predicates.h"
#include "return_adapters/throwing_adapter.h"
#include "test_utils.h"

#include <stdexcept>

using namespace return_adapters;

TEST_CASE( "Check 'generic_adapter_handler' with 'check_retval_is_not_zero' predicate", "throwing_adapter" )
{
  constexpr auto* return_int_throwing =
      RETURN_ADAPTERS_ADAPT_TO_THROWING( ra_tests::return_int, throwing::generic_adapter_handler<check_retval_is_not_zero> );
  REQUIRE( return_int_throwing );

  CHECK_NOTHROW( return_int_throwing( -1 ) );
  CHECK_THROWS_AS( return_int_throwing( 0 ), std::runtime_error );
  CHECK_THROWS_WITH( return_int_throwing( 0 ), "ra_tests::return_int failed; returned value: 0" );
}
