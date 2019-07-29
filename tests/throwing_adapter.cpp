#include "catch2/catch.hpp"

#include "return_adapters/throwing_adapter.h"

#include <stdexcept>

using namespace return_adapters;

namespace
{
enum class result
{
  success,
  failure
};

int open_file_returning_int( const result expected_result )
{
  return expected_result == result::success ? 0 : -1;
}

struct FILE;
FILE* open_file_returning_ptr( const result expected_result )
{
  return expected_result == result::success ? reinterpret_cast<FILE*>( 1 ) : nullptr;
}

}  // namespace

TEST_CASE( "Check throw_if_nonzero_result handler", "throwing_adapter" )
{
  constexpr auto* throwing_func =
      RETURN_ADAPTERS_ADAPT_TO_THROWING( open_file_returning_int, generic_throwing_adapter_handler<check_ret_val_is_not_zero> );
  REQUIRE( throwing_func );

  CHECK_NOTHROW( throwing_func( result::success ) );
  CHECK_THROWS_AS( throwing_func( result::failure ), std::runtime_error );
  CHECK_THROWS_WITH( throwing_func( result::failure ), "open_file_returning_int failed; returned value was: -1" );
}