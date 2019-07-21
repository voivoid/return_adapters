#include "catch2/catch.hpp"

#include "return_adapters/throwing_adapter.h"

namespace
{
bool return_false_if_greater_than_10( int v )
{
  return v > 10;
}
}  // namespace

TEST_CASE( "Check a throwing adapted function", "throwing_adapter" )
{
}
