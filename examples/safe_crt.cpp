#include "return_adapters/predicates.h"
#include "return_adapters/retval_adapter.h"
#include "return_adapters/throwing_adapter.h"

#include <cstdio>

namespace safe_crt
{

using namespace return_adapters;
using namespace return_adapters::throwing;

using check_retval_is_not_EOF = check_retval_is_not_<EOF>;

template <typename retval_predicate>
using safe_crt_throwing_handler = generic_adapter_handler<retval_predicate, errno_str_exception_formatter, std::runtime_error>;

constexpr auto* fclose  = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fclose, safe_crt_throwing_handler<check_retval_is_zero> );
constexpr auto* feof    = retval::adapt<::feof, check_retval_is_zero>();
constexpr auto* ferror  = retval::adapt<::ferror, retval::to_optional<check_retval_is_not_zero>>();
constexpr auto* fflush  = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fflush, safe_crt_throwing_handler<check_retval_is_zero> );
constexpr auto* fgetc   = retval::adapt<::fgetc, retval::to_optional<check_retval_is_not_EOF>>();
constexpr auto* fgetpos = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fgetpos, safe_crt_throwing_handler<check_retval_is_zero> );
constexpr auto* fgets   = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fgets, safe_crt_throwing_handler<check_ret_ptr_is_not_null> );
constexpr auto* fopen   = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fopen, safe_crt_throwing_handler<check_ret_ptr_is_not_null> );
constexpr auto* fseek   = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fseek, safe_crt_throwing_handler<check_retval_is_zero> );

}  // namespace safe_crt
