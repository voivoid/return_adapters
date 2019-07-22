#include "catch2/catch.hpp"

#include "return_adapters/retval_adapter.h"

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

struct int_result_adapter
{
  bool operator()( const int result ) const
  {
    return result == 0 ? true : false;
  }
};
}  // namespace

TEST_CASE( "The adaptee function should return bool as result", "retval_adapter" )
{
  constexpr auto* open_file_adaptee = adapt_func_retval<&open_file, int_result_adapter>();
  REQUIRE( open_file_adaptee );

  CHECK( open_file_adaptee( result::success ) );
  CHECK( !open_file_adaptee( result::failure ) );
}
