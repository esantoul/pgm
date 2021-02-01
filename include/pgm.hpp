#pragma once

#include <optional>
#include <tuple>
#include <type_traits>

namespace pgm // Process Generation Model
{
  // C++20 definitions
#if __cplusplus >= 202002L
#define CPP20_IMPL
  // C++20 (and later) code

  template <typename Ret_t, typename Callable_t, typename Args_tuple_t, std::size_t... arg_idx>
  constexpr bool Task_Helper(std::index_sequence<arg_idx...>)
  {
    return std::is_invocable_r_v<Ret_t, Callable_t, std::tuple_element_t<arg_idx, Args_tuple_t>...>;
  }

  template <typename Ret_t, typename Callable_t, typename Args_tuple_t>
  concept Task = Task_Helper<Ret_t, Callable_t, Args_tuple_t>(std::make_index_sequence<std::tuple_size_v<Args_tuple_t>>());

  template <typename Callable_t, typename Args_tuple_t, std::size_t... arg_idx>
  constexpr bool Condition_Helper(std::index_sequence<arg_idx...>)
  {
    using result_t = std::invoke_result_t<Callable_t, std::tuple_element_t<arg_idx, Args_tuple_t>...>;
    return std::is_invocable_v<Callable_t, std::tuple_element_t<arg_idx, Args_tuple_t>...> && (std::is_integral_v<result_t> || std::is_enum_v<result_t>);
  }

  template <typename Callable_t, typename Args_tuple_t>
  concept Condition = Condition_Helper<Callable_t, Args_tuple_t>(std::make_index_sequence<std::tuple_size_v<Args_tuple_t>>());

#else

#if __cplusplus >= 201703L
#define CPP17_IMPL
  template <typename Callable_t, typename Proto_t>
  struct is_task;
  template <typename Callable_t, typename Ret_t, typename... Args_t>
  struct is_task<Callable_t, Ret_t(Args_t...)>
  {
    static constexpr bool value = std::is_invocable_r_v<std::conditional_t<std::is_same_v<Ret_t, bool>, bool, std::optional<Ret_t>>, Callable_t, Args_t...>;
  };

  template <typename Callable_t, typename Proto_t>
  bool is_task_v = is_task<Callable_t, Proto_t>::value;

  template <typename Callable_t, typename Proto_t>
  struct is_condition;
  template <typename Callable_t, typename Ret_t, typename... Args_t>
  struct is_condition<Callable_t, Ret_t(Args_t...)>
  {
    static constexpr bool value = std::is_invocable_v<Callable_t, Args_t...> && (std::is_integral_v<std::invoke_result_t<Callable_t, Args_t...>> || std::is_enum_v<std::invoke_result_t<Callable_t, Args_t...>>);
  };

  template <typename Callable_t, typename Proto_t>
  constexpr bool is_condition_v = is_condition<Callable_t, Proto_t>::value;

#else
#error Program must be compiled with C++17 or more recent
#endif

#endif

  // Process & Switcher helper methods/templates
  class Helper
  {
  public:
    template <typename Proto_t>
    struct task_ret;
    template <typename Ret_t, typename... Args_t>
    struct task_ret<Ret_t(Args_t...)>
    {
      using type = std::conditional_t<std::is_same_v<Ret_t, bool>, bool, std::optional<Ret_t>>;
    };

    template <typename Proto_t>
    using task_ret_t = typename task_ret<Proto_t>::type;

    template <typename Proto_t>
    struct args_tuple;
    template <typename Ret_t, typename... Args>
    struct args_tuple<Ret_t(Args...)>
    {
      using type = std::tuple<Args...>;
    };

    template <typename Proto_t>
    using args_tuple_t = typename args_tuple<Proto_t>::type;

    template <typename Callable_t, typename Proto_t>
    struct cond_ret;
    template <typename Callable_t, typename Ret_t, typename... Args>
    struct cond_ret<Callable_t, Ret_t(Args...)>
    {
      using type = std::invoke_result_t<Callable_t, Args...>;
    };

    template <typename Callable_t, typename Proto_t>
    using cond_ret_t = typename cond_ret<Callable_t, Proto_t>::type;
  };

  // Process definition
  template <typename Proto_t, typename... Task_t>
#ifdef CPP20_IMPL
  requires(Task<Helper::task_ret_t<Proto_t>, Task_t, Helper::args_tuple_t<Proto_t>> &&...)
#endif
    class Process
  {
  public:
#ifdef CPP17_IMPL
    template <typename = std::enable_if_t<std::conjunction_v<is_task<Task_t, Proto_t>...>>>
#endif
    constexpr Process(Task_t... t)
      : mT{ t... }
    {
    }

    template <typename... Args_t>
    constexpr Helper::task_ret_t<Proto_t> operator()(Args_t... args) const
    {
      return execute_tasks(std::make_tuple(args...), std::make_index_sequence<sizeof...(Task_t)>());
    }

    template <typename New_Task_t>
    constexpr Process<Proto_t, Task_t..., New_Task_t> append(New_Task_t c) const
    {
      return append_helper(c, std::make_index_sequence<sizeof...(Task_t)>());
    }

    template <typename New_Task_t>
    constexpr Process<Proto_t, Task_t..., New_Task_t> operator<<(New_Task_t c) const
    {
      return append_helper(c, std::make_index_sequence<sizeof...(Task_t)>());
    }

  private:
    template <typename Other_Callable, std::size_t... idx>
    constexpr Process<Proto_t, Task_t..., Other_Callable> append_helper(Other_Callable c, std::index_sequence<idx...>) const
    {
      return { std::get<idx>(mT)..., c };
    }

    template <typename Args_tuple_t, std::size_t... idx>
    constexpr Helper::task_ret_t<Proto_t> execute_tasks(Args_tuple_t targs, std::index_sequence<idx...>) const
    {
      Helper::task_ret_t<Proto_t> ret{};
      if (((ret = std::apply(std::get<idx>(mT), targs)) || ...))
        void();
      return ret;
    }

    std::tuple<Task_t...> mT;
  };

  // Switcher definition
  template <typename Proto_t>
  struct Prototype
  {
  };

  template <typename Proto_t, typename CondFun_t, typename DefFun_t, typename... CaseFun_t>
#ifdef CPP20_IMPL
  requires Condition<CondFun_t, Helper::args_tuple_t<Proto_t>> && (Task<Helper::task_ret_t<Proto_t>, CaseFun_t, Helper::args_tuple_t<Proto_t>> && ...)
#endif
    class Switcher
  {
  public:
#ifdef CPP17_IMPL
    template <typename = std::enable_if_t<std::conjunction_v<is_condition<CondFun_t, Proto_t>, is_task<DefFun_t, Proto_t>, is_task<CaseFun_t, Proto_t>...>>>
#endif
    constexpr Switcher(Prototype<Proto_t>, CondFun_t cond, DefFun_t def, std::pair<Helper::cond_ret_t<CondFun_t, Proto_t>, CaseFun_t>... cases)
      : mCFun{ cond }, mDFun{ def }, mCVals{ cases.first... }, mCFuns{ cases.second... }
    {
    }

    template <typename... Args_t>
    constexpr Helper::task_ret_t<Proto_t> operator()(Args_t... args) const { return visit_cases(Helper::args_tuple_t<Proto_t>{args...}, std::make_index_sequence<sizeof...(CaseFun_t)>()); }

  private:
    template <std::size_t... idx>
    constexpr Helper::task_ret_t<Proto_t> visit_cases(Helper::args_tuple_t<Proto_t> args_tuple, std::index_sequence<idx...>) const
    {
      Helper::task_ret_t<Proto_t> ret{};
      const Helper::cond_ret_t<CondFun_t, Proto_t> condition{ std::apply(mCFun, args_tuple) };
      if (((((mCVals[idx] == condition) && ((ret = std::apply(std::get<idx>(mCFuns), args_tuple)) || (true)))) || ...))
        return ret;
      else
        return std::apply(mDFun, args_tuple);
    }

    CondFun_t mCFun;
    DefFun_t mDFun;
    Helper::cond_ret_t<CondFun_t, Proto_t> mCVals[sizeof...(CaseFun_t)];
    std::tuple<CaseFun_t...> mCFuns;
  };
} // namespace pgm
