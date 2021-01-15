#include <cstddef>
#include <cstdint>
#include <tuple>

namespace u8an
{
#if __cplusplus >= 202002L
    // C++20 (and later) code

  template <typename Callable_t>
  concept Task = std::is_invocable_r_v<bool, Callable_t, const uint8_t* &, std::size_t&>;

  template <typename Callable_t>
  concept SwitchInput = std::is_invocable_r_v<uint8_t, Callable_t, const uint8_t*, std::size_t&>;

  template <typename Callable_t>
  concept SwitchCase = std::is_invocable_r_v<std::pair<bool,bool>, Callable_t, uint8_t, const uint8_t*, std::size_t>;
  
#endif

  template <typename ... Task_t>
#if __cplusplus >= 202002L
  requires (Task<Task_t> && ...)
#endif
  class Process
  {
  public:
    constexpr Process(Task_t... t) : mT{t...} {}
    constexpr bool operator()(const uint8_t* bytes, std::size_t length) const
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
    constexpr bool execute_tasks(const uint8_t* bytes, std::size_t length, std::index_sequence<idx...>) const
    {
      return (std::get<idx>(mT)(bytes, length) || ...);
    }

    std::tuple<Task_t...> mT;
  };

  template <typename Fun_t, typename ... Case_t>
#if __cplusplus >= 202002L
  requires SwitchInput<Fun_t> && (SwitchCase<Case_t> && ...)
#endif
  class Switch
  {
  public:
    constexpr Switch(Fun_t f, Case_t... c) : mF{f}, mC{c...} {}

    constexpr bool operator()(const uint8_t* bytes, std::size_t length) const
    {
      return visit_cases(bytes, length, std::make_index_sequence<sizeof...(Case_t)>());
    }

  private:
    template <std::size_t ... idx>
    constexpr bool visit_cases(const uint8_t* bytes, std::size_t length, std::index_sequence<idx...>) const
    {
      bool ret = false;
      const uint8_t val = mF(bytes, length);
      auto visitor = [&](auto&& c){
        auto [shouldReturn, shouldBreak] = c(val, bytes, length);
        ret = shouldReturn;
        return shouldReturn || shouldBreak;
      };
      if ((visitor(std::get<idx>(mC)) || ...))
        return ret;
      return false;
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
    return [=](uint8_t val, const uint8_t* bytes, std::size_t length) -> std::pair<bool,bool> {
      if (v == val)
        return {cb(bytes, length), true};
      return {false, false};
    };
  };

  template <typename Callback_t>
#if __cplusplus >= 202002L
  requires Task<Callback_t>
#endif
  constexpr auto Default(Callback_t cb)
  {
    return [=](uint8_t val, const uint8_t* bytes, std::size_t length) -> std::pair<bool,bool> {
      return {cb(bytes, length), true};
    };
  };

}

/*

//////////////////////////////////////////////////////////////////////////////////////

#include <cstdio>

void printsuccess(const uint8_t* bytes, const std::size_t &length)
{
  puts("Success");
}

uint8_t signature[] = {1,2};

std::tuple<bool, const uint8_t*, std::size_t> mymatcher(const uint8_t* bytes, const std::size_t &length)
{
  const uint8_t* cpy = bytes;
  if (length >= 2)
    for (const auto& el: signature)
      if (el != *cpy++)
      {
        puts("Signature did not match!");
        return {false, nullptr, 0};
      }
  return {true, bytes + 2, length - 2};
}

//////////////////////////////////////////////////////////////////////////////////////

constexpr auto printfirstbyte = [](const uint8_t* bytes, std::size_t length)
{
  if (!length)
    return true;
  else
    printf("%d, ", *bytes);
  return false;
};

template <std::size_t len>
constexpr auto strip = [](const uint8_t* &bytes, std::size_t &length){
  if (length > len)
  {
    bytes += len;
    length -= len;
  }
  else
  {
    bytes += length;
    length = 0;
  }
  return false;
};

constexpr auto p = u8an::Process{}
                        << printfirstbyte
                        << strip<1>
                        << printfirstbyte
                        << strip<1>;

constexpr auto p2 = p << p << p << p;

bool funfun(const uint8_t*, std::size_t);

using u8an::Switch;
using u8an::Case;
using u8an::Default;

constexpr auto myS = Switch { [](const uint8_t* bytes, std::size_t){ return *bytes & 3; },
  Case(0, [](const uint8_t* bytes, std::size_t){ puts("It's 0!"); return false; }),
  Case(1, [](const uint8_t* bytes, std::size_t){ puts("It's 1!"); return false; }),
  //Case(2, funfun),
  Default([](const uint8_t* bytes, std::size_t){ puts("No match found :("); return false; }),
};

int main()
{
  uint8_t bytes[] = {1, 2, 3, 4, 5};
  //interpret_customizable(bytes, sizeof(bytes));
  p2(bytes, sizeof(bytes));
  puts("");
  myS(bytes, sizeof(bytes));
  return 0;
}

//*/
