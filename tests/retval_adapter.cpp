#include "catch2/catch.hpp"

#include "return_adapters/predicates.h"
#include "return_adapters/retval_adapter.h"
#include "test_utils.h"

#include <cmath>
#include <tuple>
#include <type_traits>

using namespace return_adapters;

namespace
{

const double nan_value = std::numeric_limits<double>::quiet_NaN();

struct map_result_to_double
{
  double operator()( const int v ) const
  {
    return static_cast<double>( v );
  }
};

struct check_is_not_nan
{
  bool operator()( const double d ) const
  {
    return !std::isnan( d );
  }
};

}  // namespace

TEST_CASE( "Adapted function returns true if error_code is success, false otherwise", "[retval_adapter]" )
{
  constexpr auto adapted = map_retval<&test_utils::read_file, check_retval_is_<test_utils::error_code::success>>;
  STATIC_REQUIRE( adapted );
  STATIC_REQUIRE( std::is_same<bool ( *const )( test_utils::error_code ), decltype( adapted )>::value );

  CHECK( adapted( test_utils::error_code::success ) );
  CHECK( !adapted( test_utils::error_code::unknown ) );
  CHECK( !adapted( test_utils::error_code::permission_denied ) );
}

TEST_CASE( "Adapted function returns a sum mapped to double", "[retval_adapter]" )
{
  constexpr int ( *sum_ints )( int, int ) = &test_utils::sum<int, int, int>;

  constexpr auto adapted = map_retval<sum_ints, map_result_to_double>;
  STATIC_REQUIRE( adapted );
  STATIC_REQUIRE( std::is_same<double ( *const )( int, int ), decltype( adapted )>::value );

  CHECK( Approx( 0.0 ) == adapted( -1, 1 ) );
  CHECK( Approx( 3.0 ) == adapted( 1, 2 ) );
}

TEST_CASE( "Adapted function returns empty std::optional if result is NaN value", "[retval_adapter]" )
{
  constexpr auto adapted = map_retval<&test_utils::sum<double, double, double>, retval::to_optional<check_is_not_nan>>;
  STATIC_REQUIRE( adapted );
  STATIC_REQUIRE( std::is_same<std::optional<double> ( *const )( double, double ), decltype( adapted )>::value );

  const auto result = adapted( -2.0, -3.0 );
  REQUIRE( result );
  CHECK( Approx( -5.0 ) == *result );

  CHECK( !adapted( 1, nan_value ) );
  CHECK( !adapted( nan_value, 1 ) );
}

TEMPLATE_TEST_CASE( "Static check adapted function types",
                    "[retval_adapter]",
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

  struct dummy_handler
  {
    Ret operator()( Ret ) const
    {
      throw;
    };
  };

  constexpr auto adapted = map_retval<&test_utils::dummy<Ret, Arg>, dummy_handler>;
  STATIC_REQUIRE( adapted );
  STATIC_REQUIRE( std::is_same<Ret ( *const )( Arg ), decltype( adapted )>::value );
}

TEST_CASE( "Adapted function returns sum of varargs", "[retval_adapter]" )
{
  constexpr auto adapted = map_retval<&test_utils::sum_varargs, check_retval_is_zero>;

  const size_t num_of_args = 5;
  CHECK( adapted( num_of_args, 1, 2, 3, 4, -10 ) );  // sum of these numbrers are zero
  CHECK( !adapted( num_of_args, 1, 2, 3, 4, 5 ) );   // sum of these numbrers are not zero
}
