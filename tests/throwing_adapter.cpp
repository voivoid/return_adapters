#include "catch2/catch.hpp"

#include "return_adapters/predicates.h"
#include "return_adapters/throwing_adapter.h"
#include "test_utils.h"

#include <ostream>
#include <tuple>

using namespace return_adapters;

namespace
{
struct dummy_handler
{
  template <typename R>
  void operator()( const char* const f, R&& )
  {
  }
};

TEMPLATE_TEST_CASE( "Static check throwing adapted function types",
                    "[throwing_adapter]",
                    (std::tuple<int, int>),
                    (std::tuple<int&, int&>),
                    (std::tuple<int&&, int&&>),
                    (std::tuple<int, int&>),
                    (std::tuple<int&, int>),
                    (std::tuple<int, int&&>),
                    (std::tuple<int&&, int>),
                    (std::tuple<int&, int&&>),
                    (std::tuple<int&&, int&>))
{
  using Ret = std::tuple_element_t<0, TestType>;
  using Arg = std::tuple_element_t<1, TestType>;

  constexpr auto* adapted = RETURN_ADAPTERS_ADAPT_TO_THROWING( (test_utils::dummy<Ret, Arg>), dummy_handler );
  STATIC_REQUIRE( adapted );
  STATIC_REQUIRE( std::is_same<Ret ( *const )( Arg ), decltype( adapted )>::value );
}

TEST_CASE( "Adapted function throws exception", "[throwing_adapter]" )
{
  constexpr auto* adapted = RETURN_ADAPTERS_ADAPT_TO_THROWING_GENERIC( test_utils::read_file, test_utils::check_is_success );
  STATIC_REQUIRE( adapted );

  CHECK_NOTHROW( adapted( test_utils::error_code::success ) );
  CHECK_THROWS_AS( adapted( test_utils::error_code::permission_denied ), throwing::generic_exception );
  CHECK_THROWS_WITH( adapted( test_utils::error_code::permission_denied ),
                     "test_utils::read_file failed; returned value: error_code::permission_denied" );
}

}  // namespace
