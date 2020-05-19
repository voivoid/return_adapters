#include "test_utils.h"

#include <iostream>

namespace return_adapters
{
namespace test_utils
{
std::ostream& operator<<( std::ostream& s, error_code c )
{
  switch ( c )
  {
    case error_code::success: s << "error_code::success"; break;
    case error_code::unknown: s << "error_code::unknown"; break;
    case error_code::permission_denied: s << "error_code::permission_denied"; break;
  }

  return s;
}
}  // namespace test_utils
}  // namespace return_adapters