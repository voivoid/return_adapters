#include "catch2/catch.hpp"

#include "return_adapters/non_throwing_adapter.h"

#include <stdexcept>

namespace
{
  int dec_or_throw_if_greater_than_10(const int x)
  {
    if (x > 10)
    {
      throw std::runtime_error("test exception");
    }

    return x - 1;
  }

  void try_to_do_things(const bool should_throw)
  {
    if (should_throw)
    {
      throw std::runtime_error("test exception");
    }
  }
}

TEST_CASE("Check a non-throwing adapted non-void function", "non_throwing_adapter")
{
  constexpr auto* dec_if_less_than_11 = adapt_to_non_throwing_func<dec_or_throw_if_greater_than_10>();
  REQUIRE(dec_if_less_than_11);
  
  SECTION("checking a case with no exception")
  {
    auto result = dec_if_less_than_11(10);
    REQUIRE(result);
    REQUIRE(*result == 9);
  }

  SECTION("checking a case with an exception")
  {
    auto result = dec_if_less_than_11(11);
    REQUIRE(!result);
  }
}

TEST_CASE("Check a non-throwing adapted void function", "non_throwing_adapter")
{
  constexpr auto* do_things_without_throwing = adapt_to_non_throwing_func<try_to_do_things>();
  REQUIRE(do_things_without_throwing);

  SECTION("checking a case with no exception")
  {
    auto result = do_things_without_throwing(false);
    REQUIRE(result);
  }

  SECTION("checking a case with an exception")
  {
    auto result = do_things_without_throwing(true);
    REQUIRE(!result);
  }
}
