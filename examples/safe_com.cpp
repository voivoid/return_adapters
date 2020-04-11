#include "return_adapters/predicates.h"
#include "return_adapters/throwing_adapter.h"
#include "return_adapters/out_retval_adapter.h"

#include <string>

#include <comdef.h>

namespace safe_com
{

using namespace return_adapters;

struct hresult_str_exception_formatter
{
  std::string operator()( const char* const adaptee_func_name, const HRESULT result ) const
  {
    return throwing::details::format_error_message( adaptee_func_name, "hresult", std::to_string( result ) );
  }
};

struct is_hresult_succeeded_predicate
{
    bool operator()( const HRESULT result ) const
    {
      return SUCCEEDED( result );
    }
};

using safe_com_throwing_handler = throwing::generic_adapter_handler<is_hresult_succeeded_predicate, throwing::generic_exception_factory<hresult_str_exception_formatter, std::runtime_error>>;

constexpr auto CoInitialize     = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::CoInitialize, safe_com_throwing_handler );
constexpr auto CoCreateGuid     = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::CoCreateGuid, safe_com_throwing_handler );
constexpr auto CoCreateInstance = turn_outarg_to_optional_retval<&::CoCreateInstance, is_hresult_succeeded_predicate>();
constexpr auto IIDFromString    = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::IIDFromString, safe_com_throwing_handler );
constexpr auto StringFromIID    = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::StringFromIID, safe_com_throwing_handler );
}  // namespace safe_com
