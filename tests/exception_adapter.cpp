#include "catch2/catch.hpp"

#include "return_adapters/exception_adapter.h"
#include "test_utils.h"

#include <stdexcept>

using namespace return_adapters;

namespace
{
struct my_exception
{
};

struct exception_transformer
{
  void operator()()
  {
    try
    {
      throw;
    }
    catch ( const std::exception& )
    {
      throw my_exception{};
    }
  }
};
}  // namespace

TEST_CASE( "test map_exception", "exception_adapter" )
{
  constexpr auto* adapted = map_exception<&ra_tests::throwing_function, exception_transformer>;
  REQUIRE( adapted );

  CHECK_NOTHROW( adapted( ra_tests::throw_mode::dont_throw_exception ) );
  CHECK_THROWS_AS( adapted( ra_tests::throw_mode::throw_std_exception ), my_exception );
  CHECK_THROWS_AS( adapted( ra_tests::throw_mode::throw_non_std_exception ), ra_tests::non_std_exception );
}