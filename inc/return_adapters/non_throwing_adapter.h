#pragma once

#include <optional>
#include <type_traits>

namespace details
{
  template <auto adaptee_func, typename FuncType>
  struct non_throwing_adapter;

  template <auto adaptee_func, typename... Args>
  struct non_throwing_adapter<adaptee_func, void(*)(Args...)>
  {
    static bool non_throwing_func(Args... args)
    {
      try
      {
        adaptee_func(std::forward<Args>(args)...);
        return true;
      }
      catch (...)
      {
        return false;
      }
    }
  };

  template <auto adaptee_func, typename Ret, typename... Args>
  struct non_throwing_adapter<adaptee_func, Ret(*)(Args...)>
  {
    static std::optional<Ret> non_throwing_func(Args... args)
    {
      try
      {
        return adaptee_func(std::forward<Args>(args)...);
      }
      catch (...)
      {
        return {};
      }
    }
  };
}



template <auto* func>
constexpr auto* adapt_to_non_throwing_func()
{
  return &details::non_throwing_adapter<func, decltype(func)>::non_throwing_func;
}