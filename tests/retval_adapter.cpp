#include "catch2/catch.hpp"

#include "return_adapters/predicates.h"
#include "return_adapters/retval_adapter.h"
#include "test_utils.h"

#include <type_traits>

using namespace return_adapters;

TEST_CASE( "Check retval adapter with 'check_retval_is_zero' predicate", "retval_adapter" )
{
  constexpr auto adapted = map_retval<&ra_tests::ret_int_take_int, check_retval_is_zero>;
  static_assert( std::is_same<bool ( *const )( int ), decltype( adapted )>::value, "Adapted type mismatches" );

  REQUIRE( adapted );

  CHECK( adapted( 0 ) );
  CHECK( !adapted( -1 ) );

  // TODO: test all ret_int_take_int combinations
  {
    constexpr auto* adapted = map_retval<&ra_tests::ret_int_take_intref, check_retval_is_zero>;
    static_assert( std::is_same<bool ( *const )( int& ), decltype( adapted )>::value, "Adapted type mismatches" );
  }

  {
    constexpr auto* adapted = map_retval<&ra_tests::ret_int_take_intcref, check_retval_is_zero>;
    static_assert( std::is_same<bool ( *const )( const int& ), decltype( adapted )>::value, "Adapted type mismatches" );
  }
}

TEST_CASE( "Check varargs retval adapter", "retval_adapter" )
{
  constexpr auto adapted = map_retval<&ra_tests::add_nums, check_retval_is_zero>;

  CHECK( false == adapted( 5, 1, 2, 3, 4, 5 ) );   // sum of these numbrers are not zero
  CHECK( true == adapted( 5, 1, 2, 3, 4, -10 ) );  // sum of these numbrers are zero
}

// TODO: write tests to to_optional adapter
