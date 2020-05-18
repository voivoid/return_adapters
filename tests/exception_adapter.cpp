#include "catch2/catch.hpp"

#include "return_adapters/exception_adapter.h"
#include "test_utils.h"

#include <stdexcept>

using namespace return_adapters;
using namespace test_utils;

namespace
{
struct my_exception : std::exception
{
};

struct std_exception_to_my_exception
{
  void operator()() const
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

TEST_CASE( "test map_exception", "[exception_adapter]" )
{
  constexpr auto* adapted = map_exception<&throwing_function, std_exception_to_my_exception>;
  REQUIRE( adapted );

  CHECK_NOTHROW( adapted( throw_mode::dont_throw ) );
  CHECK_THROWS_AS( adapted( throw_mode::throw_std_based ), my_exception );
  CHECK_THROWS_AS( adapted( throw_mode::throw_non_std_based ), non_std_based_exception );
}

}  // namespace
