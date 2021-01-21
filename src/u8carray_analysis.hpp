#include <cstddef>
#include <cstdint>
#include <tuple>

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
    constexpr __attribute__((always_inline)) bool operator()(const uint8_t* bytes, std::size_t length) const
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
    constexpr __attribute__((always_inline)) bool execute_tasks(const uint8_t* bytes, std::size_t length, std::index_sequence<idx...>) const
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

    constexpr __attribute__((always_inline)) bool operator()(const uint8_t* bytes, std::size_t length) const
    {
      return visit_cases(bytes, length, std::make_index_sequence<sizeof...(Case_t)>());
    }

  private:
    template <std::size_t ... idx>
    constexpr __attribute__((always_inline)) bool visit_cases(const uint8_t* bytes, std::size_t length, std::index_sequence<idx...>) const
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

//////////////////////////////////////////////////////////////////////////////////////

/*
#include <cstdio>

constexpr bool currlen(const uint8_t*, std::size_t length)
{
  printf("> %d\n", length);
  return false;
}


//////////////////////////////////////////////////////////////////////////////////////

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
          static bool method(const uint8_t*, std::size_t);
        };
        struct SampleDataPacket : NotInstantiable
        {
          static constexpr uint8_t value = 0x02;
          static bool method(const uint8_t*, std::size_t);
        };
        struct SampleDumpRequest : NotInstantiable
        {
          static constexpr uint8_t value = 0x03;
          static bool method(const uint8_t*, std::size_t);
        };
        struct MidiTimeCode : NotInstantiable
        {
          static constexpr uint8_t value = 0x04;
          static bool method(const uint8_t*, std::size_t);
        };
        struct SampleDumpExtensions : NotInstantiable
        {
          static constexpr uint8_t value = 0x05;
          static bool method(const uint8_t*, std::size_t);
        };
        struct GeneralInformation : NotInstantiable
        {
          static constexpr uint8_t value = 0x06;
          static bool method(const uint8_t*, std::size_t);
        };
        struct FileDump : NotInstantiable
        {
          static constexpr uint8_t value = 0x07;
          static bool method(const uint8_t*, std::size_t);
        };
        struct MidiTuningStandard : NotInstantiable
        {
          static constexpr uint8_t value = 0x08;
          static bool method(const uint8_t*, std::size_t);
        };
        struct GeneralMidi : NotInstantiable
        {
          static constexpr uint8_t value = 0x09;
          static bool method(const uint8_t*, std::size_t);
        };
        struct EndOfFile : NotInstantiable
        {
          static constexpr uint8_t value = 0x7B;
          static bool method(const uint8_t*, std::size_t);
        };
        struct Wait : NotInstantiable
        {
          static constexpr uint8_t value = 0x7C;
          static bool method(const uint8_t*, std::size_t);
        };
        struct Cancel : NotInstantiable
        {
          static constexpr uint8_t value = 0x7D;
          static bool method(const uint8_t*, std::size_t);
        };
        struct NAK : NotInstantiable
        {
          static constexpr uint8_t value = 0x7E;
          static bool method(const uint8_t*, std::size_t);
        };
        struct ACK : NotInstantiable
        {
          static constexpr uint8_t value = 0x7F;
          static bool method(const uint8_t*, std::size_t);
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
        static constexpr auto method = u8an::Process{}    <<
          u8an::StripBytes<1>                             <<
          u8an::CheckLength<1>                            <<
          u8an::SwitchHelper::MakeFromCaseList<CaseList>(
            u8an::GetByteMask<1,0xFF>,
            u8an::Default([](auto...){ return true; })
          );
      };
    
    struct UniRT : NotInstantiable
    {
      static constexpr uint8_t value = 0x7F;
      static bool method(const uint8_t*, std::size_t);
    };

    private:
      using CaseList = std::tuple<
        UniNonRT,
        UniRT>;

    public:
      // @return true if there is a match
      static bool specificMatchFunction(const uint8_t*&, std::size_t &);
      static bool interpretSpecificSysEx(const uint8_t*, std::size_t lenght);

      static constexpr uint8_t value = 0xF0;
      static constexpr auto method = u8an::Process{}    <<
        u8an::StripStatus                               <<
        u8an::CheckLength<1>                            <<
        u8an::SwitchHelper::MakeFromCaseList<CaseList>(
          u8an::GetFirstByte,
          u8an::Default( u8an::Process{}  <<
            specificMatchFunction         <<
            interpretSpecificSysEx
          )
        );
    };
    struct MTC : NotInstantiable
    {
      static constexpr uint8_t value = 0xF1;
      static bool method(const uint8_t*, std::size_t);
    };
    struct Songpos : NotInstantiable
    {
      static constexpr uint8_t value = 0xF2;
      static bool method(const uint8_t*, std::size_t);
    };
    struct SongSel : NotInstantiable
    {
      static constexpr uint8_t value = 0xF3;
      static bool method(const uint8_t*, std::size_t);
    };
    struct Tune : NotInstantiable
    {
      static constexpr uint8_t value = 0xF6;
      static bool method(const uint8_t*, std::size_t);
    };

    // System Real Time
    struct Clock : NotInstantiable
    {
      static constexpr uint8_t value = 0xF8;
      static bool method(const uint8_t*, std::size_t);
    };
    struct Start : NotInstantiable
    {
      static constexpr uint8_t value = 0xFA;
      static bool method(const uint8_t*, std::size_t);
    };
    struct Continue : NotInstantiable
    {
      static constexpr uint8_t value = 0xFB;
      static bool method(const uint8_t*, std::size_t);
    };
    struct Stop : NotInstantiable
    {
      static constexpr uint8_t value = 0xFC;
      static bool method(const uint8_t*, std::size_t);
    };
    struct ActiveSensing : NotInstantiable
    {
      static constexpr uint8_t value = 0xFE;
      static bool method(const uint8_t*, std::size_t);
    };
    struct SystemReset : NotInstantiable
    {
      static constexpr uint8_t value = 0xFF;
      static bool method(const uint8_t*, std::size_t);
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
    static constexpr auto method = u8an::Process{}    <<
      u8an::SwitchHelper::MakeFromCaseList<CaseList>(
        u8an::GetFirstByte,
        u8an::Default([](auto...){ return true; })
      );
  };

private:
  using CaseList = std::tuple<
    SystemMessage>;

public:

  static constexpr auto Interpret = u8an::Process{} <<
    u8an::CheckLength<1>                            <<
    u8an::SwitchHelper::MakeFromCaseList<CaseList>(
      u8an::GetFirstByteMask<0xF0>,
      u8an::Default([](auto...){ return true; })
    );
};


//////////////////////////////////////////////////////////////////////////////////////

bool analyzer(const uint8_t *bytes, std::size_t length)
{
  return MidiBytes::Interpret(bytes, length);
}

//*/

//////////////////////////////////////////////////////////////////////////////////////
/*

void prtrem(const char* description, const uint8_t* bytes, std::size_t len)
{
  std::printf("%s\n", description);
  for (std::size_t i = 0; i < len; ++i)
    std::printf("%02X ", bytes[i]);
  std::printf("\n");
}

bool MidiBytes::SystemMessage::SysEx::UniNonRT::SampleDumpHeader::method(const uint8_t* bytes, std::size_t length) { prtrem("SampleDumpHeader", bytes, length); return true; }
bool MidiBytes::SystemMessage::SysEx::UniNonRT::SampleDataPacket::method(const uint8_t* bytes, std::size_t length) { prtrem("SampleDataPacket", bytes, length); return true; }
bool MidiBytes::SystemMessage::SysEx::UniNonRT::SampleDumpRequest::method(const uint8_t* bytes, std::size_t length) { prtrem("SampleDumpRequest", bytes, length); return true; }
bool MidiBytes::SystemMessage::SysEx::UniNonRT::MidiTimeCode::method(const uint8_t* bytes, std::size_t length) { prtrem("MidiTimeCode", bytes, length); return true; }
bool MidiBytes::SystemMessage::SysEx::UniNonRT::SampleDumpExtensions::method(const uint8_t* bytes, std::size_t length) { prtrem("SampleDumpExtensions", bytes, length); return true; }
bool MidiBytes::SystemMessage::SysEx::UniNonRT::GeneralInformation::method(const uint8_t* bytes, std::size_t length) { prtrem("GeneralInformation", bytes, length); return true; }
bool MidiBytes::SystemMessage::SysEx::UniNonRT::FileDump::method(const uint8_t* bytes, std::size_t length) { prtrem("FileDump", bytes, length); return true; }
bool MidiBytes::SystemMessage::SysEx::UniNonRT::MidiTuningStandard::method(const uint8_t* bytes, std::size_t length) { prtrem("MidiTuningStandard", bytes, length); return true; }
bool MidiBytes::SystemMessage::SysEx::UniNonRT::GeneralMidi::method(const uint8_t* bytes, std::size_t length) { prtrem("GeneralMidi", bytes, length); return true; }
bool MidiBytes::SystemMessage::SysEx::UniNonRT::EndOfFile::method(const uint8_t* bytes, std::size_t length) { prtrem("EndOfFile", bytes, length); return true; }
bool MidiBytes::SystemMessage::SysEx::UniNonRT::Wait::method(const uint8_t* bytes, std::size_t length) { prtrem("Wait", bytes, length); return true; }
bool MidiBytes::SystemMessage::SysEx::UniNonRT::Cancel::method(const uint8_t* bytes, std::size_t length) { prtrem("Cancel", bytes, length); return true; }
bool MidiBytes::SystemMessage::SysEx::UniNonRT::NAK::method(const uint8_t* bytes, std::size_t length) { prtrem("NAK", bytes, length); return true; }
bool MidiBytes::SystemMessage::SysEx::UniNonRT::ACK::method(const uint8_t* bytes, std::size_t length) { prtrem("ACK", bytes, length); return true; }

bool MidiBytes::SystemMessage::SysEx::UniRT::method(const uint8_t* bytes, std::size_t length) { prtrem("UniRT", bytes, length); return true; }

bool MidiBytes::SystemMessage::SysEx::specificMatchFunction(const uint8_t* &bytes, std::size_t &length) { prtrem("specificMatchFunction", bytes, length); return true; }
bool MidiBytes::SystemMessage::SysEx::interpretSpecificSysEx(const uint8_t* bytes, std::size_t length) { prtrem("interpretSpecificSysEx", bytes, length); return true; }

bool MidiBytes::SystemMessage::MTC::method(const uint8_t* bytes, std::size_t length) { prtrem("MTC", bytes, length); return true; }
bool MidiBytes::SystemMessage::Songpos::method(const uint8_t* bytes, std::size_t length) { prtrem("Songpos", bytes, length); return true; }
bool MidiBytes::SystemMessage::SongSel::method(const uint8_t* bytes, std::size_t length) { prtrem("SongSel", bytes, length); return true; }
bool MidiBytes::SystemMessage::Tune::method(const uint8_t* bytes, std::size_t length) { prtrem("Tune", bytes, length); return true; }
bool MidiBytes::SystemMessage::Clock::method(const uint8_t* bytes, std::size_t length) { prtrem("Clock", bytes, length); return true; }
bool MidiBytes::SystemMessage::Start::method(const uint8_t* bytes, std::size_t length) { prtrem("Start", bytes, length); return true; }
bool MidiBytes::SystemMessage::Continue::method(const uint8_t* bytes, std::size_t length) { prtrem("Continue", bytes, length); return true; }
bool MidiBytes::SystemMessage::Stop::method(const uint8_t* bytes, std::size_t length) { prtrem("Stop", bytes, length); return true; }
bool MidiBytes::SystemMessage::ActiveSensing::method(const uint8_t* bytes, std::size_t length) { prtrem("ActiveSensing", bytes, length); return true; }
bool MidiBytes::SystemMessage::SystemReset::method(const uint8_t* bytes, std::size_t length) { prtrem("SystemReset", bytes, length); return true; }

//*/
/*

int main()
{
  //uint8_t bytes[] = {0xF0, 0x7E, 0x69, 0x07, 0x03, 0x42, 'B', 'I', 'N', ' ', 'e', 'r', 'a', 'e', '_', 't', 'o', 'u', 'c', 'h', '_', 's', 'o', 'f', 't', 0xF7};
  uint8_t bytes[] = {0xF0, 0x7E, 0x7F, 0x06, 0x01, 0xF7};
  MidiBytes::Interpret(bytes, std::size(bytes));
  return 0;
}

//*/
