#include "catch2/catch.hpp"

#include "return_adapters/out_retval_adapter.h"
#include "test_utils.h"

using namespace return_adapters;
using namespace return_adapters::out_retval;

struct foo
{
};

TEST_CASE( "Check out_retval adapter", "out_retval_adapter" )
{
  constexpr auto adapted = adapt<&ra_tests::dec_if_positive_via_out_argument, foo>();
  REQUIRE( adapted );

  SECTION( "true" )
  {
    auto res = adapted( 10 );
    REQUIRE( res );
    CHECK( *res == 9 );
  }

  SECTION( "false" )
  {
    auto res = adapted( -1 );
    CHECK( !res );
  }
}
