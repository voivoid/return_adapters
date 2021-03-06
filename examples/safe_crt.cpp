#include "return_adapters/predicates.h"
#include "return_adapters/raii_adapter.h"
#include "return_adapters/retval_adapter.h"
#include "return_adapters/throwing_adapter.h"

#include <cstdio>

namespace safe_crt
{

namespace
{
struct check_fread_result
{
  auto operator()( const size_t fread_result, const size_t fread_count_arg, FILE* const fread_stream_arg ) const
  {
    return fread_result == fread_count_arg || !ferror( fread_stream_arg );
  }
};

struct check_fscanf_result
{
  template <typename... VarArgs>
  auto operator()( const int fscanf_result,
                   FILE* const /*fscanf_stream_arg*/,
                   const char* const /*fscanf_format_arg*/,
                   VarArgs... fscanf_var_args ) const
  {
    return fscanf_result != EOF && fscanf_result == sizeof...( fscanf_var_args );
  }
};
}  // namespace

using namespace return_adapters;

template <typename retval_predicate>
using safe_crt_throwing_handler = throwing::generic_adapter_handler<retval_predicate>;

constexpr auto fclose  = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fclose, safe_crt_throwing_handler<check_retval_is_zero> );
constexpr auto feof    = map_retval<::feof, check_retval_is_zero>;
constexpr auto ferror  = map_retval<::ferror, check_retval_is_zero>;
constexpr auto fflush  = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fflush, safe_crt_throwing_handler<check_retval_is_zero> );
constexpr auto fgetc   = map_retval<::fgetc, retval::to_optional<check_retval_is_not_<EOF>>>;
constexpr auto fgetpos = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fgetpos, safe_crt_throwing_handler<check_retval_is_zero> );
constexpr auto fgets   = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fgets, safe_crt_throwing_handler<check_ret_ptr_is_not_null> );
constexpr auto fopen =
    to_unique_ptr_with_f_deleter<RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fopen, safe_crt_throwing_handler<check_ret_ptr_is_not_null> ),
                                 safe_crt::fclose>;
constexpr auto fprintf = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fprintf, safe_crt_throwing_handler<check_retval_is_not_negative> );
constexpr auto fputc   = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fputc, safe_crt_throwing_handler<check_retval_is_not_<EOF>> );
constexpr auto fputs   = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fputs, safe_crt_throwing_handler<check_retval_is_not_<EOF>> );
constexpr auto fread   = RETURN_ADAPTERS_ADAPT_TO_THROWING_WITH_INDICES( ::fread, safe_crt_throwing_handler<check_fread_result>, 2, 3 );
constexpr auto freopen = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::freopen, safe_crt_throwing_handler<check_ret_ptr_is_not_null> );
constexpr auto fscanf  = RETURN_ADAPTERS_ADAPT_TO_THROWING_WITH_ALL_ARGS( ::fscanf, safe_crt_throwing_handler<check_fscanf_result> );
constexpr auto fseek   = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fseek, safe_crt_throwing_handler<check_retval_is_zero> );

}  // namespace safe_crt

namespace
{

void example()
{
  auto file = safe_crt::fopen( "file.txt", "w" );  // returns std::unique_ptr<FILE> with safe_crt::fclose deleter
  safe_crt::fprintf( file.get(), "HELLO, %s\n", "WORLD" );
}

}  // namespace
