
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

////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------//

//                                    EXAMPLES

//--------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////

#include <cstddef>
#include <cstdint>

// Switcher utility function for MIDI interpretation
class SwitcherMaker
{
public:
  template <typename Proto_t, typename CaseTuple_t, typename CondFun_t, typename Default_t>
  static constexpr auto Interpreter(CondFun_t cf, Default_t df)
  {
    return InterpreterHelper<Proto_t, CaseTuple_t>(cf, df, std::make_index_sequence<std::tuple_size_v<CaseTuple_t>>());
  }

  template <typename Proto_t, typename CaseTuple_t, typename CondFun_t>
  static constexpr auto Interpreter(CondFun_t cf)
  {
    return InterpreterHelper<Proto_t, CaseTuple_t>(
      cf, [](auto...) -> pgm::Helper::task_ret_t<Proto_t> { return {}; }, std::make_index_sequence<std::tuple_size_v<CaseTuple_t>>());
  }

private:
  template <typename Proto_t, typename CaseTuple_t, typename CondFun_t, typename Default_t, std::size_t... case_idx>
  static constexpr auto InterpreterHelper(CondFun_t cf, Default_t df, std::index_sequence<case_idx...>)
  {
    return pgm::Switcher{
        pgm::Prototype<Proto_t>{},
        cf,
        df,
        std::make_pair(
            std::tuple_element_t<case_idx, CaseTuple_t>::value,
            std::tuple_element_t<case_idx, CaseTuple_t>::method)... };
  }
};

// Tasks Definitions
template <std::size_t len>
constexpr auto CheckLength = [](const uint8_t *, std::size_t length) {
  return length < len;
};

template <std::size_t len>
constexpr auto StripBytes = [](const uint8_t *&bytes, std::size_t &length) {
  bytes = length > len ? bytes + len : bytes + length;
  length = length > len ? length - len : 0;
  return false;
};

constexpr auto StripStatus = [](const uint8_t *&bytes, std::size_t &length) {
  if (length < 2)
    return true;
  ++bytes, length -= 2;
  return false;
};

template <std::size_t idx, uint8_t mask>
constexpr auto GetByteMask = [](const uint8_t *bytes, std::size_t) -> uint8_t {
  return bytes[idx] & mask;
};

template <uint8_t mask>
constexpr auto GetFirstByteMask = GetByteMask<0, mask>;

constexpr auto GetFirstByte = GetFirstByteMask<0xFF>;

////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------//

//*         SWITCH FOR MIDI Message analysis / Process return type demo

//--------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////

struct NotInstantiable
{
private:
  constexpr NotInstantiable() {}
};
struct MidiBytes : NotInstantiable
{
  struct SystemMessage : NotInstantiable
  {
    // System Common
    struct SysEx : NotInstantiable
    {
      struct UniNonRT : NotInstantiable
      {
        struct SampleDumpHeader : NotInstantiable
        {
          static constexpr uint8_t value = 0x01;
          static bool method(const uint8_t *, std::size_t);
        };
        struct SampleDataPacket : NotInstantiable
        {
          static constexpr uint8_t value = 0x02;
          static bool method(const uint8_t *, std::size_t);
        };
        struct SampleDumpRequest : NotInstantiable
        {
          static constexpr uint8_t value = 0x03;
          static bool method(const uint8_t *, std::size_t);
        };
        struct MidiTimeCode : NotInstantiable
        {
          static constexpr uint8_t value = 0x04;
          static bool method(const uint8_t *, std::size_t);
        };
        struct SampleDumpExtensions : NotInstantiable
        {
          static constexpr uint8_t value = 0x05;
          static bool method(const uint8_t *, std::size_t);
        };
        struct GeneralInformation : NotInstantiable
        {
          static constexpr uint8_t value = 0x06;
          static bool method(const uint8_t *, std::size_t);
        };
        struct FileDump : NotInstantiable
        {
          static constexpr uint8_t value = 0x07;
          static bool method(const uint8_t *, std::size_t);
        };
        struct MidiTuningStandard : NotInstantiable
        {
          static constexpr uint8_t value = 0x08;
          static bool method(const uint8_t *, std::size_t);
        };
        struct GeneralMidi : NotInstantiable
        {
          static constexpr uint8_t value = 0x09;
          static bool method(const uint8_t *, std::size_t);
        };
        struct EndOfFile : NotInstantiable
        {
          static constexpr uint8_t value = 0x7B;
          static bool method(const uint8_t *, std::size_t);
        };
        struct Wait : NotInstantiable
        {
          static constexpr uint8_t value = 0x7C;
          static bool method(const uint8_t *, std::size_t);
        };
        struct Cancel : NotInstantiable
        {
          static constexpr uint8_t value = 0x7D;
          static bool method(const uint8_t *, std::size_t);
        };
        struct NAK : NotInstantiable
        {
          static constexpr uint8_t value = 0x7E;
          static bool method(const uint8_t *, std::size_t);
        };
        struct ACK : NotInstantiable
        {
          static constexpr uint8_t value = 0x7F;
          static bool method(const uint8_t *, std::size_t);
        };

      private:
        using CaseList = std::tuple<
          SampleDumpHeader,
          SampleDataPacket,
          SampleDumpRequest,
          MidiTimeCode,
          SampleDumpExtensions,
          GeneralInformation,
          FileDump,
          MidiTuningStandard,
          GeneralMidi,
          EndOfFile,
          Wait,
          Cancel,
          NAK,
          ACK>;

      public:
        static constexpr uint8_t value = 0x7E;
        static constexpr auto method = pgm::Process<bool(const uint8_t *&, std::size_t &)>{}
          << StripBytes<1>
          << CheckLength<1>
          << SwitcherMaker::Interpreter<bool(const uint8_t *, std::size_t), CaseList>(
            GetByteMask<1, 0xFF>,
            [](auto...) { return true; });
      };

      struct UniRT : NotInstantiable
      {
        static constexpr uint8_t value = 0x7F;
        static bool method(const uint8_t *, std::size_t);
      };

    private:
      using CaseList = std::tuple<
        UniNonRT,
        UniRT>;

    public:
      // @return false if there is a match else true
      static bool specificMatchFunction(const uint8_t *&, std::size_t &);
      static bool interpretSpecificSysEx(const uint8_t *, std::size_t lenght);
      static constexpr uint8_t value = 0xF0;
      static constexpr auto method =
        pgm::Process<bool(const uint8_t *&, std::size_t &)>{}
        << StripStatus
        << CheckLength<1>
        << SwitcherMaker::Interpreter<bool(const uint8_t *, std::size_t), CaseList>(
          GetFirstByte,
          pgm::Process<bool(const uint8_t *&, std::size_t &)>{} << specificMatchFunction << interpretSpecificSysEx);
    };
    struct MTC : NotInstantiable
    {
      static constexpr uint8_t value = 0xF1;
      static bool method(const uint8_t *, std::size_t);
    };
    struct Songpos : NotInstantiable
    {
      static constexpr uint8_t value = 0xF2;
      static bool method(const uint8_t *, std::size_t);
    };
    struct SongSel : NotInstantiable
    {
      static constexpr uint8_t value = 0xF3;
      static bool method(const uint8_t *, std::size_t);
    };
    struct Tune : NotInstantiable
    {
      static constexpr uint8_t value = 0xF6;
      static bool method(const uint8_t *, std::size_t);
    };
    // System Real Time
    struct Clock : NotInstantiable
    {
      static constexpr uint8_t value = 0xF8;
      static bool method(const uint8_t *, std::size_t);
    };
    struct Start : NotInstantiable
    {
      static constexpr uint8_t value = 0xFA;
      static bool method(const uint8_t *, std::size_t);
    };
    struct Continue : NotInstantiable
    {
      static constexpr uint8_t value = 0xFB;
      static bool method(const uint8_t *, std::size_t);
    };
    struct Stop : NotInstantiable
    {
      static constexpr uint8_t value = 0xFC;
      static bool method(const uint8_t *, std::size_t);
    };
    struct ActiveSensing : NotInstantiable
    {
      static constexpr uint8_t value = 0xFE;
      static bool method(const uint8_t *, std::size_t);
    };
    struct SystemReset : NotInstantiable
    {
      static constexpr uint8_t value = 0xFF;
      static bool method(const uint8_t *, std::size_t);
    };

  private:
    using CaseList = std::tuple<
      SysEx,
      MTC,
      Songpos,
      SongSel,
      Tune,
      Clock,
      Start,
      Continue,
      Stop,
      ActiveSensing,
      SystemReset>;

  public:
    static constexpr uint8_t value = 0xF0;
    static constexpr auto method = pgm::Process<bool(const uint8_t *&, std::size_t &)>{}
      << SwitcherMaker::Interpreter<bool(const uint8_t *, std::size_t), CaseList>(
        GetFirstByte,
        [](auto...) { return true; });
  };

private:
  using CaseList = std::tuple<
    SystemMessage>;

public:
  static constexpr auto Interpret = pgm::Process<bool(const uint8_t *&, std::size_t &)>{}
    << CheckLength<1>
    << SwitcherMaker::Interpreter<bool(const uint8_t *, std::size_t), CaseList>(
      GetFirstByteMask<0xF0>,
      [](auto...) { return true; });
};

////////////////////////////////////////////////////////////////////////////////////

bool analyzer(const uint8_t *bytes, std::size_t length)
{
  return MidiBytes::Interpret(bytes, length);
}

#include <cstdio>
void prtrem(const char *description, const uint8_t *bytes, std::size_t len)
{
  std::printf("%s\n", description);
  for (std::size_t i = 0; i < len; ++i)
    std::printf("%02X ", bytes[i]);
  std::printf("\n");
}

bool MidiBytes::SystemMessage::SysEx::UniNonRT::SampleDumpHeader::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("SampleDumpHeader", bytes, length);
  return true;
}
bool MidiBytes::SystemMessage::SysEx::UniNonRT::SampleDataPacket::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("SampleDataPacket", bytes, length);
  return true;
}
bool MidiBytes::SystemMessage::SysEx::UniNonRT::SampleDumpRequest::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("SampleDumpRequest", bytes, length);
  return true;
}
bool MidiBytes::SystemMessage::SysEx::UniNonRT::MidiTimeCode::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("MidiTimeCode", bytes, length);
  return true;
}
bool MidiBytes::SystemMessage::SysEx::UniNonRT::SampleDumpExtensions::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("SampleDumpExtensions", bytes, length);
  return true;
}
bool MidiBytes::SystemMessage::SysEx::UniNonRT::GeneralInformation::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("GeneralInformation", bytes, length);
  return true;
}
bool MidiBytes::SystemMessage::SysEx::UniNonRT::FileDump::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("FileDump", bytes, length);
  return true;
}
bool MidiBytes::SystemMessage::SysEx::UniNonRT::MidiTuningStandard::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("MidiTuningStandard", bytes, length);
  return true;
}
bool MidiBytes::SystemMessage::SysEx::UniNonRT::GeneralMidi::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("GeneralMidi", bytes, length);
  return true;
}
bool MidiBytes::SystemMessage::SysEx::UniNonRT::EndOfFile::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("EndOfFile", bytes, length);
  return true;
}
bool MidiBytes::SystemMessage::SysEx::UniNonRT::Wait::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("Wait", bytes, length);
  return true;
}
bool MidiBytes::SystemMessage::SysEx::UniNonRT::Cancel::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("Cancel", bytes, length);
  return true;
}
bool MidiBytes::SystemMessage::SysEx::UniNonRT::NAK::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("NAK", bytes, length);
  return true;
}
bool MidiBytes::SystemMessage::SysEx::UniNonRT::ACK::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("ACK", bytes, length);
  return true;
}
bool MidiBytes::SystemMessage::SysEx::UniRT::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("UniRT", bytes, length);
  return true;
}
bool MidiBytes::SystemMessage::SysEx::specificMatchFunction(const uint8_t *&bytes, std::size_t &length)
{
  prtrem("specificMatchFunction", bytes, length);
  return true;
}
bool MidiBytes::SystemMessage::SysEx::interpretSpecificSysEx(const uint8_t *bytes, std::size_t length)
{
  prtrem("interpretSpecificSysEx", bytes, length);
  return true;
}
bool MidiBytes::SystemMessage::MTC::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("MTC", bytes, length);
  return true;
}
bool MidiBytes::SystemMessage::Songpos::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("Songpos", bytes, length);
  return true;
}
bool MidiBytes::SystemMessage::SongSel::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("SongSel", bytes, length);
  return true;
}
bool MidiBytes::SystemMessage::Tune::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("Tune", bytes, length);
  return true;
}
bool MidiBytes::SystemMessage::Clock::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("Clock", bytes, length);
  return true;
}
bool MidiBytes::SystemMessage::Start::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("Start", bytes, length);
  return true;
}
bool MidiBytes::SystemMessage::Continue::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("Continue", bytes, length);
  return true;
}
bool MidiBytes::SystemMessage::Stop::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("Stop", bytes, length);
  return true;
}
bool MidiBytes::SystemMessage::ActiveSensing::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("ActiveSensing", bytes, length);
  return true;
}
bool MidiBytes::SystemMessage::SystemReset::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("SystemReset", bytes, length);
  return true;
}

int main()
{
  //uint8_t bytes[] = {0xF0, 0x7E, 0x69, 0x07, 0x03, 0x42, 'B', 'I', 'N', ' ', 'e', 'r', 'a', 'e', '_', 't', 'o', 'u', 'c', 'h', '_', 's', 'o', 'f', 't', 0xF7};
  uint8_t bytes[] = { 0xF0, 0x7E, 0x7F, 0x06, 0x01, 0xF7 };
  MidiBytes::Interpret(bytes, std::size(bytes));
  return 0;
}

/*/

constexpr auto mycond = [](const uint8_t *, std::size_t length) -> std::size_t { return length; };
constexpr auto mydefault = [](const uint8_t *, std::size_t length) -> std::optional<int> { if (length < 12) return length; return {}; };

struct C_A
{
  static constexpr std::size_t value = 3;
  static constexpr auto method = [](auto...) { return 33; };
};

struct C_B
{
  static constexpr std::size_t value = 4;
  static constexpr auto method = [](auto...) { return 42; };
};

struct C_C
{
  static constexpr std::size_t value = 5;
  static constexpr auto method = [](auto...) { return 77; };
};

using CaseList = std::tuple<C_A, C_B, C_C>;

constexpr auto mySwitcher = SwitcherMaker::Interpreter<int(const uint8_t *, std::size_t), CaseList>(
  mycond,
  mydefault);

constexpr auto p = pgm::Process<int(const uint8_t *&, std::size_t &)>{}
<< [](const uint8_t *, std::size_t length) -> std::optional<int> { if (length < 2) return -1; return {}; }
<< [](const uint8_t *&bytes, std::size_t &length) -> std::optional<int> { bytes++, length--; return {}; }
<< mySwitcher;

std::optional<int> szf(const uint8_t *bytes, std::size_t length)
{
  return p(bytes, length);
}

enum class EN
{
  red,
  blue,
  yellow
};

using tested_t = bool;

int main()
{
  (void)StripStatus;
  (void)GetFirstByte;
  std::uint8_t bb[6];
  return szf(bb, sizeof(bb)).value_or(-99);
}

//*/
////////////////////////////////////////////////////////////////////////////////////
