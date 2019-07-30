#pragma once

#include "return_adapters/throwing_adapter.h"

#include <cstdio>

using nullptr_handler = return_adapters::throwing::generic_adapter_handler<
  return_adapters::throwing::check_ptr_is_not_null,
  return_adapters::throwing::errno_str_exception_formatter,
  std::runtime_error>;

using nonzero_handler = return_adapters::throwing::generic_adapter_handler<
  return_adapters::throwing::check_ret_val_is_zero,
  return_adapters::throwing::errno_str_exception_formatter,
  std::runtime_error>;

constexpr auto* safe_fopen = RETURN_ADAPTERS_ADAPT_TO_THROWING( fopen, nullptr_handler );
constexpr auto* safe_fclose = RETURN_ADAPTERS_ADAPT_TO_THROWING( fclose, nonzero_handler );
constexpr auto* safe_fseek = RETURN_ADAPTERS_ADAPT_TO_THROWING( fseek, nonzero_handler );