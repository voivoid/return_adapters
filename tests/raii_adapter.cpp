#include "catch2/catch.hpp"

#include "return_adapters/raii_adapter.h"
#include "test_utils.h"

#include <cassert>

using namespace return_adapters;

namespace
{
struct foo
{
  foo( bool& dtor_called_flag ) : m_dtor_called_flag( dtor_called_flag )
  {
    assert( !m_dtor_called_flag );
  }

  ~foo()
  {
    m_dtor_called_flag = true;
  }

private:
  bool& m_dtor_called_flag;
};

foo* make_foo( bool& dtor_called_flag )
{
  return new foo( dtor_called_flag );
}

}  // namespace

TEST_CASE( "Check raii adapter with default deleter", "raii_adapter" )
{
  constexpr auto* make_foo_adapted = raii::adapt<&make_foo>;

  bool dtor_called_flag = false;

  {
    auto foo = make_foo_adapted( dtor_called_flag );
    CHECK( !dtor_called_flag );
  }

  CHECK( dtor_called_flag );
}
