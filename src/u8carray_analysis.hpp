#include <cstddef>
#include <cstdint>
#include <tuple>

//#define FORCE_INLINE __attribute__((always_inline))
#ifdef __clang__
#define FORCE_INLINE        __attribute__((always_inline))
#define FORCE_INLINE_LAMBDA
#define DETAIL false
#else
#ifdef __GNUC__
#define FORCE_INLINE        __attribute__((always_inline))
#define FORCE_INLINE_LAMBDA __attribute__((always_inline))
#define DETAIL true
#else
#define FORCE_INLINE        
#define FORCE_INLINE_LAMBDA 
#define DETAIL false
#endif
#endif

namespace u8an
{
  // C++20 definitions
#if __cplusplus >= 202002L
    // C++20 (and later) code

  template <typename Callable_t>
  concept Task = std::is_invocable_r_v<bool, Callable_t, const uint8_t* &, std::size_t&>;

  template <typename Callable_t>
  concept SwitchInput = std::is_invocable_r_v<uint8_t, Callable_t, const uint8_t*, std::size_t&>;

  template <typename Callable_t>
  concept SwitchCase = std::is_invocable_r_v<std::pair<bool,bool>, Callable_t, uint8_t, const uint8_t*, std::size_t>;
  
#endif

  // Process Definition
  template <typename ... Task_t>
#if __cplusplus >= 202002L
  requires (Task<Task_t> && ...)
#endif
  class Process
  {
  public:
    constexpr Process(Task_t... t) : mT{t...} {}
    constexpr FORCE_INLINE bool operator()(const uint8_t* bytes, std::size_t length) const
    {
      return execute_tasks(bytes, length, std::make_index_sequence<sizeof...(Task_t)>());
    }

    template <typename New_Task_t>
#if __cplusplus >= 202002L
    requires Task<New_Task_t>
#endif
    constexpr Process<Task_t..., New_Task_t> operator<<(New_Task_t c) const
    {
      return append_helper(c, std::make_index_sequence<sizeof...(Task_t)>());
    }

  private:
    template <typename Other_Callable, std::size_t... idx>
    constexpr Process<Task_t..., Other_Callable> append_helper(Other_Callable c, std::index_sequence<idx...>) const
    {
      return {std::get<idx>(mT)..., c};
    }

    template <std::size_t ... idx>
    constexpr FORCE_INLINE bool execute_tasks(const uint8_t* bytes, std::size_t length, std::index_sequence<idx...>) const
    {
      return (std::get<idx>(mT)(bytes, length) || ...);
    }

    std::tuple<Task_t...> mT;
  };

  // Tasks Definitions

  template <std::size_t len>
  constexpr auto CheckLength = [](const uint8_t* bytes, std::size_t length){
    return length < len;
  };

  template <std::size_t len>
  constexpr auto StripBytes = [](const uint8_t* &bytes, std::size_t &length){
    bytes = length > len ? bytes + len : bytes + length;
    length = length > len ? length - len : 0;
    return false;
  };

  constexpr auto StripStatus = [](const uint8_t* &bytes, std::size_t &length){
    if (length < 2)
      return true;
    ++bytes, length -= 2;
    return false;
  };

  template <std::size_t idx, uint8_t mask>
  constexpr auto GetByteMask = [](const uint8_t* bytes, std::size_t){
    return bytes[idx] & mask;
  };

  template <uint8_t mask>
  constexpr auto GetFirstByteMask = GetByteMask<0, mask>;

  constexpr auto GetFirstByte = GetFirstByteMask<0xFF>;

  // Task Utility
  template <typename Callback_t>
#if __cplusplus >= 202002L
    requires Task<Callback_t>
#endif
  constexpr auto invertReturnVal(Callback_t c)
  {
    return [=](const uint8_t* &bytes, std::size_t& length){
      return !c(bytes, length);
    };
  }

  template <typename Fun_t, typename ... Case_t>
#if __cplusplus >= 202002L
  requires SwitchInput<Fun_t> && (SwitchCase<Case_t> && ...)
#endif
  class Switch
  {
  public:
    constexpr Switch(Fun_t f, Case_t... c) : mF{f}, mC{c...} {}

    constexpr FORCE_INLINE bool operator()(const uint8_t* bytes, std::size_t length) const
    {
      return visit_cases(bytes, length, std::make_index_sequence<sizeof...(Case_t)>());
    }

  private:
    template <std::size_t ... idx>
    constexpr FORCE_INLINE bool visit_cases(const uint8_t* bytes, std::size_t length, std::index_sequence<idx...>) const
    {
      bool ret = false;
      const uint8_t val = mF(bytes, length);
      if constexpr (DETAIL)
      {
        std::pair<bool,bool> i{};
        if ((((ret = (i = std::get<idx>(mC)(val, bytes, length)).first || i.second)) || ...)) void();
        }
      else
      {
        auto visitor = [&](auto&& c) FORCE_INLINE_LAMBDA {
          auto [shouldReturn, shouldBreak] = c(val, bytes, length);
          ret = shouldReturn;
          return ret || shouldBreak;
        };      
        if ((visitor(std::get<idx>(mC)) || ...)) void();
      }
      return ret;
    }

    Fun_t mF;
    std::tuple<Case_t...> mC;
  };

  template <typename Callback_t>
#if __cplusplus >= 202002L
  requires Task<Callback_t>
#endif
  constexpr auto Case(uint8_t v, Callback_t cb)
  {
    return [=](uint8_t val, const uint8_t* bytes, std::size_t length) FORCE_INLINE_LAMBDA {
      if (v == val)
        return std::make_pair(cb(bytes, length), true);
      return std::make_pair(false, false);
    };
  };

  template <typename Callback_t>
#if __cplusplus >= 202002L
  requires Task<Callback_t>
#endif
  constexpr auto Default(Callback_t cb)
  {
    return [=](uint8_t val, const uint8_t* bytes, std::size_t length) FORCE_INLINE_LAMBDA {
      return std::make_pair(cb(bytes, length), true);
    };
  };

  class SwitchHelper
  {
  public:
    template <typename Tuple_t, typename Fun_t>
    static constexpr auto MakeFromCaseList(Fun_t f)
    {
      return MakeFromCaseListHelper<Tuple_t>(f, std::make_index_sequence<std::tuple_size_v<Tuple_t>>());
    }

    template <typename Tuple_t, typename Fun_t, typename Default_t>
    static constexpr auto MakeFromCaseList(Fun_t f, Default_t d)
    {
      return MakeFromCaseListHelper<Tuple_t>(f, d, std::make_index_sequence<std::tuple_size_v<Tuple_t>>());
    }

  private:
    template <typename Tuple_t, typename Fun_t, std::size_t ... idx>
    static constexpr auto MakeFromCaseListHelper(Fun_t f, std::index_sequence<idx...>)
    {
      return Switch{f, Case(std::tuple_element<idx, Tuple_t>::type::value, std::tuple_element<idx, Tuple_t>::type::method)...};
    }

    template <typename Tuple_t, typename Fun_t, typename Default_t, std::size_t ... idx>
    static constexpr auto MakeFromCaseListHelper(Fun_t f, Default_t d, std::index_sequence<idx...>)
    {
      return Switch{f, Case(std::tuple_element<idx, Tuple_t>::type::value, std::tuple_element<idx, Tuple_t>::type::method)..., d};
    }
  };

}
