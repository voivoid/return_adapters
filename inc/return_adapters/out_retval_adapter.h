#pragma once


namespace return_adapters
{

namespace out_retval
{

namespace details
{
template <auto* adaptee_func, typename FuncType, typename OutRetValAdapter>
struct out_retval_adapter;

template <auto* adaptee_func, typename Ret, typename... Args, typename OutRetValAdapter>
struct out_retval_adapter<adaptee_func, Ret ( * )( Args... ), OutRetValAdapter>
{
  static auto retval_adapted_func( Args... args )
  {
      return nullptr;
  }
};

}

template <auto* func, typename OutRetValAdapter>
constexpr auto* adapt()
{
  return &details::out_retval_adapter<func, decltype( func ), OutRetValAdapter>::retval_adapted_func;
}

}

}
