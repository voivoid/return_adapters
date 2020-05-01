#include "catch2/catch.hpp"

#include "return_adapters/predicates.h"
#include "return_adapters/throwing_adapter.h"
#include "test_utils.h"

#include <stdexcept>

using namespace return_adapters;

namespace
{
struct tmp_handler
{
  int operator()( const char* const func_name, int result, float arg )
  {
    return 111;
  }

  int operator()( const char* const func_name, int result, int arg1, float arg2 )
  {
    return 222;
  }
};
}  // namespace

TEST_CASE( "Check 'generic_adapter_handler' with 'check_retval_is_not_zero' predicate", "throwing_adapter" )
{
  constexpr auto* return_int_throwing = RETURN_ADAPTERS_ADAPT_TO_THROWING_GENERIC( ra_tests::ret_int_take_int, check_retval_is_not_zero );
  REQUIRE( return_int_throwing );

  CHECK_NOTHROW( return_int_throwing( -1 ) );
  CHECK_THROWS_AS( return_int_throwing( 0 ), std::runtime_error );
  CHECK_THROWS_WITH( return_int_throwing( 0 ), "ra_tests::ret_int_take_int failed; returned value: 0" );
}

TEST_CASE( "Check 'make_throwing_with_arg'", "throwing_adapter" )
{
  // TODO: write tests

  constexpr auto* return_int_throwing = RETURN_ADAPTERS_ADAPT_TO_THROWING_WITH_ARG( ra_tests::sum, tmp_handler, float );
  CHECK( 111 == return_int_throwing( 2, 3.0 ) );
  constexpr auto* return_int_throwing2 = RETURN_ADAPTERS_ADAPT_TO_THROWING_WITH_INDICES( ra_tests::sum, tmp_handler, 0, 1 );
  CHECK( 222 == return_int_throwing2( 2, 3.0 ) );
}
