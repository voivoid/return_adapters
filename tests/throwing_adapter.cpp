#include "catch2/catch.hpp"

#include "return_adapters/predicates.h"
#include "return_adapters/throwing_adapter.h"
#include "test_utils.h"

#include <stdexcept>

using namespace return_adapters;
using namespace return_adapters::throwing;

TEST_CASE( "Check throw_if_nonzero_result handler", "throwing_adapter" )
{
  constexpr auto* throwing_func =
      RETURN_ADAPTERS_ADAPT_TO_THROWING( ra_tests::return_int, generic_adapter_handler<check_retval_is_not_zero> );
  REQUIRE( throwing_func );

  CHECK_NOTHROW( throwing_func( ra_tests::int_value::non_zero) );
  CHECK_THROWS_AS( throwing_func( ra_tests::int_value::zero ), std::runtime_error );
  CHECK_THROWS_WITH( throwing_func( ra_tests::int_value::zero ), "ra_tests::return_int failed; returned value: 0" );
}
