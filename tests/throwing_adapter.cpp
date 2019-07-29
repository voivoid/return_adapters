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

int open_file( const result expected_result )
{
  return expected_result == result::success ? 0 : -1;
}

struct throw_if_nonzero_result
{
    void operator()( const int result ) const
    {
        if( result != 0 )
        {
            throw std::runtime_error( "operation failed" );
        }
    }
};

}  // namespace

TEST_CASE( "Check a throwing adapted function", "throwing_adapter" )
{
    constexpr auto* throwing_func = adapt_to_throwing_func<&open_file, throw_if_nonzero_result>();
    REQUIRE( throwing_func );

    CHECK_NOTHROW( throwing_func( result::success ));
    CHECK_THROWS_AS( throwing_func( result::failure ), std::runtime_error );
}
