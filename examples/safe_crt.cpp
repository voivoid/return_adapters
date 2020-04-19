#include "return_adapters/predicates.h"
#include "return_adapters/raii_adapter.h"
#include "return_adapters/retval_adapter.h"
#include "return_adapters/throwing_adapter.h"

#include <cstdio>

namespace safe_crt
{

using namespace return_adapters;

template <typename retval_predicate>
using safe_crt_throwing_handler = throwing::generic_adapter_handler<retval_predicate>;

constexpr auto fclose  = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fclose, safe_crt_throwing_handler<check_retval_is_zero> );
constexpr auto feof    = map_retval<::feof, check_retval_is_zero>();
constexpr auto ferror  = map_retval<::ferror, retval::to_optional<check_retval_is_not_zero>>();
constexpr auto fflush  = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fflush, safe_crt_throwing_handler<check_retval_is_zero> );
constexpr auto fgetc   = map_retval<::fgetc, retval::to_optional<check_retval_is_not_<EOF>>>();
constexpr auto fgetpos = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fgetpos, safe_crt_throwing_handler<check_retval_is_zero> );
constexpr auto fgets   = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fgets, safe_crt_throwing_handler<check_ret_ptr_is_not_null> );
constexpr auto fopen   = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fopen, safe_crt_throwing_handler<check_ret_ptr_is_not_null> );
//             fprintf
constexpr auto fputc = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fputc, safe_crt_throwing_handler<check_retval_is_not_<EOF>> );
constexpr auto fputs = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fputs, safe_crt_throwing_handler<check_retval_is_not_<EOF>> );
//             fread
constexpr auto freopen = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::freopen, safe_crt_throwing_handler<check_ret_ptr_is_not_null> );
//             fscanf
constexpr auto fseek = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fseek, safe_crt_throwing_handler<check_retval_is_zero> );

}  // namespace safe_crt

namespace
{

void example()
{
  safe_crt::fopen( "file.txt", "r" );
}

}  // namespace
