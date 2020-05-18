#include "catch2/catch.hpp"

#include "return_adapters/raii_adapter.h"
#include "test_utils.h"

#include <cassert>

using namespace return_adapters;

namespace
{
struct Resource
{
  Resource( bool& is_destructed ) : _is_destructed( is_destructed )
  {
    assert( !is_destructed );
  };
  bool& _is_destructed;
};

struct ResourceWithDtor : private Resource
{
  using Resource::Resource;
  ~ResourceWithDtor()
  {
    _is_destructed = true;
  }
};

template <typename R>
R* alloc_resource( bool& is_destructed )
{
  return new R( is_destructed );
}

void free_resource( Resource* resource )
{
  resource->_is_destructed = true;
  delete resource;
}

struct free_resource_deleter
{
  void operator()( Resource* resource ) const
  {
    free_resource( resource );
  }
};


template <auto adapted>
void check_resource_is_freed()
{
  STATIC_REQUIRE( adapted );

  bool is_destructed = false;
  {
    auto resource = adapted( is_destructed );
  }

  CHECK( is_destructed );
}

REGISTER_TEST_CASE( check_resource_is_freed<to_unique_ptr<&alloc_resource<ResourceWithDtor>>>,
                    "Adapt return value to unique_ptr",
                    "[raii_adapter]" );

REGISTER_TEST_CASE( check_resource_is_freed<to_shared_ptr<&alloc_resource<ResourceWithDtor>>>,
                    "Adapt return value to shared_ptr",
                    "[raii_adapter]" );

REGISTER_TEST_CASE( (check_resource_is_freed<to_unique_ptr_with_f_deleter<&alloc_resource<Resource>, free_resource>>),
                    "Adapt return value to unique_ptr with function deleter",
                    "[raii_adapter]" );

REGISTER_TEST_CASE( (check_resource_is_freed<to_unique_ptr_with_deleter<&alloc_resource<Resource>, free_resource_deleter>>),
                    "Adapt return value to unique_ptr with deleter",
                    "[raii_adapter]" );

}  // namespace
