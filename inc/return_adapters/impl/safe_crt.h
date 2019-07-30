#pragma once

#include "return_adapters/throwing_adapter.h"

#include <cstdio>

namespace safe_crt
{

using namespace return_adapters::throwing;

template <typename retval_checker>
using safe_crt_throwing_handler = generic_adapter_handler<retval_checker, errno_str_exception_formatter, std::runtime_error>;

constexpr auto* fflush = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fflush, safe_crt_throwing_handler<check_ret_val_is_zero> );
constexpr auto* fopen  = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fopen, safe_crt_throwing_handler<check_ret_ptr_is_not_null> );
constexpr auto* fclose = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fclose, safe_crt_throwing_handler<check_ret_val_is_zero> );
constexpr auto* fseek  = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fseek, safe_crt_throwing_handler<check_ret_val_is_zero> );

}
