#ifndef PGM_HPP
#define PGM_HPP

/**
 * @file pgm.hpp
 * @author Etienne SANTOUL github.com/esantoul
 * @brief Process Generation Model header file (Compiles w/ GCC 7.1+ & Clang 7.0+. Requires std=c++17 or above)
 * @version 0.1
 * @date 2021-02-04
 *
 * @copyright BSD 2-Clause License. Copyright (c) 2021, Etienne SANTOUL. All rights reserved.
 */

#include <tuple>
#include <type_traits>

namespace pgm // Process Generation Model
{
#if __cplusplus < 201703L
#error Program must be compiled with C++17 or later
#endif

  template <typename Callable_t, typename Proto_t>
  struct is_task;
  template <typename Callable_t, typename Ret_t, typename... Args_t>
  struct is_task<Callable_t, Ret_t(Args_t...)>
  {
    static constexpr bool value = std::is_invocable_r_v<Ret_t, Callable_t, Args_t...>;
  };

  template <typename Callable_t, typename Proto_t>
  constexpr bool is_task_v = is_task<Callable_t, Proto_t>::value;

  template <typename Callable_t, typename Proto_t>
  struct is_condition;
  template <typename Callable_t, typename Ret_t, typename... Args_t>
  struct is_condition<Callable_t, Ret_t(Args_t...)>
  {
    using result_t = std::decay_t<std::invoke_result_t<Callable_t, Args_t...>>;
    static constexpr bool value = std::is_invocable_v<Callable_t, Args_t...> && (std::is_integral_v<result_t> || std::is_enum_v<result_t>);
  };

  template <typename Callable_t, typename Proto_t>
  constexpr bool is_condition_v = is_condition<Callable_t, Proto_t>::value;

  template <typename Proto_t>
  struct is_conditional_return;
  template <typename Ret_t, typename ... Args_t>
  struct is_conditional_return<Ret_t(Args_t...)>
  {
    static constexpr bool value = std::is_convertible_v<Ret_t, bool> && (std::is_copy_constructible_v<Ret_t> || std::is_move_constructible_v<Ret_t>) && std::is_default_constructible_v<Ret_t>;
  };

  template <typename Proto_t>
  constexpr bool is_conditional_return_v = is_conditional_return<Proto_t>::value;

  // C++20 definitions --> for better error messages
#if __cplusplus >= 202002L
#define CPP20_IMPL
  // C++20 (and later) code

  template <typename Callable_t, typename Proto_t>
  concept Task = is_task_v<Callable_t, Proto_t>;

  template <typename Callable_t, typename Proto_t>
  concept Condition = is_condition_v<Callable_t, Proto_t>;

  template <typename Proto_t>
  concept Conditional_Return = is_conditional_return_v<Proto_t>;

#endif

  /**
   * @brief Process & Switcher helper methods/templates
   */
  class Helper
  {
  public:
    /**
     * @brief A helper to get the return type of a function prototype
     * @tparam Proto_t a function prototype e.g. Return_type(Argument_types ...)
     */
    template <typename Proto_t>
    struct task_ret;
    template <typename Ret_t, typename... Args_t>
    struct task_ret<Ret_t(Args_t...)>
    {
      using type = Ret_t;
    };

    /**
     * @brief Return type of a function prototype
     * @tparam Proto_t a function prototype e.g. Return_type(Argument_types ...)
     */
    template <typename Proto_t>
    using ret_t = typename task_ret<Proto_t>::type;

    /**
     * @brief A helper to get a tuple of the arguments types of a function prototype
     * @tparam Proto_t a function prototype e.g. Return_type(Argument_types ...)
     */
    template <typename Proto_t>
    struct args_tuple;
    /**
     * @brief A helper to get a tuple type of the arguments types of a function prototype
     * @tparam Ret_t return type of the function prototype
     * @tparam Args arguments of the function prototype
     */
    template <typename Ret_t, typename... Args>
    struct args_tuple<Ret_t(Args...)>
    {
      using type = std::tuple<Args...>;
    };

    /**
     * @brief Tuple type of the arguments types of a function prototype
     * @tparam Proto_t a function prototype e.g. Return_type(Argument_types ...)
     */
    template <typename Proto_t>
    using args_tuple_t = typename args_tuple<Proto_t>::type;

    /**
     * @brief A helper to get the decayed return type of a Switcher conditional function
     * @tparam Callable_t The Switcher conditional function type
     * @tparam Proto_t The switcher prototype template argument
     */
    template <typename Callable_t, typename Proto_t>
    struct cond_ret;
    /**
     * @brief A helper to get the decayed return type of a Switcher conditional function
     * @tparam Callable_t The Switcher conditional function type
     * @tparam Ret_t The switcher prototype return type
     * @tparam Args The switcher argument types
     */
    template <typename Callable_t, typename Ret_t, typename... Args>
    struct cond_ret<Callable_t, Ret_t(Args...)>
    {
      using type = std::decay_t<std::invoke_result_t<Callable_t, Args...>>;
    };

    /**
     * @brief Decayed return type of a Switcher conditional function
     * @tparam Callable_t The Switcher conditional function type
     * @tparam Proto_t The switcher prototype template argument
     */
    template <typename Callable_t, typename Proto_t>
    using cond_ret_t = typename cond_ret<Callable_t, Proto_t>::type;
  };

  /**
   * @brief An object able to store a set of callables with the same function prototype and to sequentially apply these callables to a set of arguments
   * @tparam Proto_t Function prototype of the callables. The return type must be convertible to bool. The process stops when a callable returns a variable convertible to true
   * @tparam Task_t Type pack of the callables
   */
  template <typename Proto_t, typename... Task_t>
#ifdef CPP20_IMPL
  requires Conditional_Return<Proto_t> && (Task<Task_t, Proto_t> &&...)
#endif
    class Process
  {
  public:
#ifndef CPP20_IMPL
    template <typename = std::enable_if_t<std::conjunction_v<is_conditional_return<Proto_t>, is_task<Task_t, Proto_t>...>>>
#endif
    constexpr Process(Task_t... t)
      : mT{t...}
    {
    }

    /**
     * @brief Sequentially apply stored callables to arguments
     * @tparam Args_t Pack of argument types trivially convertible to the Process prototype argument types
     * @param args Pack of input arguments
     * @return The first callable return value convertible to true or the last callable return value
     */
    template <typename... Args_t>
    constexpr Helper::ret_t<Proto_t> operator()(Args_t... args) const
    {
      return execute_tasks(std::make_tuple(args...), std::make_index_sequence<sizeof...(Task_t)>());
    }

    /**
     * @brief Appends a callable to the Process
     * @tparam New_Task_t Next callable type
     * @param next_callable Callable to be added to the process
     * @return the Process with the added callable
     */
#ifdef CPP20_IMPL
    template <typename New_Task_t>
    requires Task<New_Task_t, Proto_t>
#else
    template <typename New_Task_t, typename = std::enable_if_t<is_task_v<New_Task_t, Proto_t>>>
#endif
    constexpr Process<Proto_t, Task_t..., New_Task_t> append(New_Task_t next_callable) const
    {
      return append_helper(next_callable, std::make_index_sequence<sizeof...(Task_t)>());
    }

    /**
     * @brief Appends a callable to the Process
     * @tparam New_Task_t Next callable type
     * @param next_callable Callable to be added to the process
     * @return the Process with the added callable
     */
#ifdef CPP20_IMPL
    template <typename New_Task_t>
    requires Task<New_Task_t, Proto_t>
#else
    template <typename New_Task_t, typename = std::enable_if_t<is_task_v<New_Task_t, Proto_t>>>
#endif
    constexpr Process<Proto_t, Task_t..., New_Task_t> operator<<(New_Task_t next_callable) const
    {
      return append_helper(next_callable, std::make_index_sequence<sizeof...(Task_t)>());
    }

  private:
    template <typename Other_Callable, std::size_t... idx>
    constexpr Process<Proto_t, Task_t..., Other_Callable> append_helper(Other_Callable c, std::index_sequence<idx...>) const
    {
      return {std::get<idx>(mT)..., c};
    }

    template <typename Args_tuple_t, std::size_t... idx>
    constexpr Helper::ret_t<Proto_t> execute_tasks(Args_tuple_t targs, std::index_sequence<idx...>) const
    {
      Helper::ret_t<Proto_t> ret{};
      if (((ret = std::apply(std::get<idx>(mT), targs)) || ...))
        void();
      return ret;
    }

    std::tuple<Task_t...> mT;
  };

  template <typename Proto_t>
  struct Prototype {};

  /**
   * @brief A switch wrapper
   * @tparam Proto_t Prototype of the callables called for each case or the default case
   * @tparam CondFun_t Type of the callable called to get the condition value of the switch. The decayed return type must be convertible to an integral or an enumeration type
   * @tparam DefFun_t Type of the callable called on switch default case
   * @tparam CaseFun_t Type pack of the callables called on each switch case
   */
  template <typename Proto_t, typename CondFun_t, typename DefFun_t, typename... CaseFun_t>
#ifdef CPP20_IMPL
  requires Condition<CondFun_t, Proto_t>
    &&Task<DefFun_t, Proto_t>
    && (Task<CaseFun_t, Proto_t> && ...)
#endif
    class Switcher
  {
  public:
    /**
    * @brief Constructs a Switcher
    * @param cond callable used in the switch conditional statement.
    * @param def callable called on the switch default case. The Proto_t prototype arguments must be trivially convertible to the callable arguments. The callable return type must be trivially convertible to an integral or enumeration type
    * @param case a pack of pairs with each pair containing an value as first and a callable as second. The value is the key for which the correponding callable will be called. The value of each pair must be trivially convertible to the conditional callable return type
    */
#ifndef CPP20_IMPL
    template <typename = std::enable_if_t<std::conjunction_v<is_condition<CondFun_t, Proto_t>, is_task<DefFun_t, Proto_t>, is_task<CaseFun_t, Proto_t>...>>>
#endif
    constexpr Switcher(Prototype<Proto_t>, CondFun_t cond, DefFun_t def, std::pair<Helper::cond_ret_t<CondFun_t, Proto_t>, CaseFun_t>... cases)
      : mCFun{cond}, mDFun{def}, mCVals{cases.first...}, mCFuns{cases.second...}
    {
    }

    /**
     * @brief Executes the switch case
     * @tparam Args_t Pack of argument types trivially convertible to the Condition, Cases & Default callables prototype argument types
     * @param args Argument pack that will be fed to the condition callable and to the match case or default callable
     * @return Return value of the matched case callable or of the default case callable
     */
    template <typename... Args_t>
    constexpr Helper::ret_t<Proto_t> operator()(Args_t... args) const { return visit_cases(Helper::args_tuple_t<Proto_t>{args...}, std::make_index_sequence<sizeof...(CaseFun_t)>()); }

  private:
    template <std::size_t... idx>
    constexpr Helper::ret_t<Proto_t> visit_cases(Helper::args_tuple_t<Proto_t> args_tuple, std::index_sequence<idx...>) const
    {
      Helper::ret_t<Proto_t> ret{};
      const Helper::cond_ret_t<CondFun_t, Proto_t> condition{std::apply(mCFun, args_tuple)};
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wparentheses" // added due to a bug in GCC versions < 9.3
      if ((((mCVals[idx] == condition) && ((ret = std::apply(std::get<idx>(mCFuns), args_tuple)) || true)) || ...))
#pragma GCC diagnostic pop
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

#endif // PGM_HPP
