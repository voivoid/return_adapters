#include "return_adapters/predicates.h"
#include "return_adapters/raii_adapter.h"
#include "return_adapters/retval_adapter.h"
#include "return_adapters/throwing_adapter.h"

#include <cstdio>

namespace safe_crt
{

namespace
{
struct fread_handler
{
  size_t operator()( const char* const func_name, const size_t result, const size_t fread_count_arg, FILE* const fread_stream_arg )
  {
    if ( result != fread_count_arg && ferror( fread_stream_arg ) )
    {
      // TODO: throw exception
    }

    return result;
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
constexpr auto fopen   = raii::adapt<RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fopen, safe_crt_throwing_handler<check_ret_ptr_is_not_null> ),
                                   raii::to_unique_ptr_with_deleter_func_<safe_crt::fclose>>;
constexpr auto fprintf = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fprintf, safe_crt_throwing_handler<check_retval_is_less_zero> );
constexpr auto fputc   = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fputc, safe_crt_throwing_handler<check_retval_is_not_<EOF>> );
constexpr auto fputs   = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fputs, safe_crt_throwing_handler<check_retval_is_not_<EOF>> );
constexpr auto fread   = RETURN_ADAPTERS_ADAPT_TO_THROWING_WITH_INDICES( ::fread, fread_handler, 2, 3 );
constexpr auto freopen = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::freopen, safe_crt_throwing_handler<check_ret_ptr_is_not_null> );
constexpr auto fscanf  = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fscanf, safe_crt_throwing_handler<check_retval_is_not_<EOF>> );
constexpr auto fseek   = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fseek, safe_crt_throwing_handler<check_retval_is_zero> );

}  // namespace safe_crt

namespace
{

void example()
{
  auto file = safe_crt::fopen( "file.txt", "w" );  // returns std::unique_ptr<FILE> with ::fclose deleter
  safe_crt::fprintf( file.get(), "HELLO, %s\n", "WORLD" );
}

}  // namespace
