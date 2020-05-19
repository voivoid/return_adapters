#include "catch2/catch.hpp"

#include "return_adapters/out_retval_adapter.h"
#include "return_adapters/predicates.h"
#include "test_utils.h"

#include <tuple>

using namespace return_adapters;

namespace
{

inline bool divide_with_1st_out_arg( int* result, int a, int b )
{
  return test_utils::divide<int, int*>( a, b, result );
}

inline bool divide_with_2nd_out_arg( int a, int* result, int b )
{
  return test_utils::divide<int, int*>( a, b, result );
}

inline bool divide_with_3rd_out_arg( int a, int b, int* result )
{
  return test_utils::divide<int, int*>( a, b, result );
}

template <auto adapted>
void check_out_retval_is_returned()
{
  STATIC_REQUIRE( adapted );

  std::optional<int> result = adapted( 6, 2 );
  CHECK( result );
  CHECK( *result == 3 );

  CHECK( !adapted( 6, 0 ) );
}

REGISTER_TEST_CASE( (check_out_retval_is_returned<turn_outarg_to_optional_retval<&test_utils::divide<int, int*>, check_retval_is_true>>),
                    "Check out return value pointer",
                    "[out_retval_adapter]" );

REGISTER_TEST_CASE( (check_out_retval_is_returned<turn_outarg_to_optional_retval<&test_utils::divide<int, int&>, check_retval_is_true>>),
                    "Check out return value reference",
                    "[out_retval_adapter]" );

REGISTER_TEST_CASE(
    (check_out_retval_is_returned<turn_outarg_to_optional_retval<&divide_with_1st_out_arg, check_retval_is_true, out_retval::first_arg>>),
    "Check out return value is first argument",
    "[out_retval_adapter]" );

REGISTER_TEST_CASE( (check_out_retval_is_returned<turn_outarg_to_optional_retval<&divide_with_1st_out_arg, check_retval_is_true, 0>>),
                    "Check out return value is 0th argument",
                    "[out_retval_adapter]" );

REGISTER_TEST_CASE( (check_out_retval_is_returned<turn_outarg_to_optional_retval<&divide_with_2nd_out_arg, check_retval_is_true, 1>>),
                    "Check out return value is 1th argument",
                    "[out_retval_adapter]" );

REGISTER_TEST_CASE( (check_out_retval_is_returned<turn_outarg_to_optional_retval<&divide_with_3rd_out_arg, check_retval_is_true, 2>>),
                    "Check out return value is 2th argument",
                    "[out_retval_adapter]" );

REGISTER_TEST_CASE(
    (check_out_retval_is_returned<turn_outarg_to_optional_retval<&divide_with_3rd_out_arg, check_retval_is_true, out_retval::last_arg>>),
    "Check out return value is last argument",
    "[out_retval_adapter]" );

TEMPLATE_TEST_CASE( "Static check turn_outarg_to_optional_retval adapted function types",
                    "[out_retval_adapter]",
                    (std::tuple<int, int, int*>),
                    (std::tuple<int, int&, int*>),
                    (std::tuple<int, int&&, int*>),
                    (std::tuple<double, double, double&>),
                    (std::tuple<double, double&, double&>),
                    (std::tuple<double, double&&, double&>))
{
  using Ret    = std::tuple_element_t<0, TestType>;
  using Arg    = std::tuple_element_t<1, TestType>;
  using OutArg = std::tuple_element_t<2, TestType>;


  constexpr auto adapted = turn_outarg_to_optional_retval<&test_utils::divide<Arg, OutArg>, check_retval_is_true>;
  STATIC_REQUIRE( adapted );
  STATIC_REQUIRE( std::is_same<std::optional<Ret> ( *const )( Arg, Arg ), decltype( adapted )>::value );
}

}  // namespace
