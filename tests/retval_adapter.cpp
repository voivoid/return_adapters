#include "catch2/catch.hpp"

#include "return_adapters/retval_adapter.h"
#include "return_adapters/predicates.h"

using namespace return_adapters;
using namespace return_adapters::retval;

namespace
{
enum class result
{
  success,
  failure
};

int open_file( const result expected_result )
{
  return expected_result == result::success ? 0 : -1;
}

}  // namespace

TEST_CASE( "The adaptee function should return bool as result", "retval_adapter" )
{
  constexpr auto* open_file_adaptee = adapt<&open_file, check_retval_is_zero>();
  REQUIRE( open_file_adaptee );

  CHECK( open_file_adaptee( result::success ) );
  CHECK( !open_file_adaptee( result::failure ) );
}
