#include "../include/pgm.hpp"
#include "optional_w.hpp"

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
      cf, [](auto...) -> pgm::Helper::ret_t<Proto_t> { return {}; }, std::make_index_sequence<std::tuple_size_v<CaseTuple_t>>());
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
            std::tuple_element_t<case_idx, CaseTuple_t>::method)...};
  }
};

enum class PARSE_STATUS
{
  UNDEFINED = 0,
  SUCCESS,
  ERROR
};

using Parse_info = info_descr<PARSE_STATUS>;

// Tasks Definitions
template <std::size_t len>
constexpr auto CheckLength = [](const uint8_t *, std::size_t length) -> Parse_info {
  return length < len ? Parse_info{PARSE_STATUS::ERROR, "Msg too short"} : Parse_info{};
};

template <std::size_t len>
constexpr auto HasLength = [](const uint8_t *, std::size_t length) -> bool {
  return length == len;
};

template <std::size_t len>
constexpr auto StripBytes = [](const uint8_t *&bytes, std::size_t &length) -> Parse_info {
  bytes = length > len ? bytes + len : bytes + length;
  length = length > len ? length - len : 0;
  return {};
};

constexpr auto StripStatus = [](const uint8_t *&bytes, std::size_t &length) -> Parse_info {
  if (length < 2)
    return {PARSE_STATUS::ERROR, "Msg too short"};
  ++bytes, length -= 2;
  return {};
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
  struct M1 : NotInstantiable
  {
    struct NoteOff : NotInstantiable
    {
      static constexpr std::uint8_t value = 0x80;
      static Parse_info method(const uint8_t *bytes, std::size_t length);
    };

    struct NoteOn : NotInstantiable
    {
      static constexpr std::uint8_t value = 0x90;
      static Parse_info method(const uint8_t *bytes, std::size_t length);
    };

    struct PolyPressure : NotInstantiable
    {
      static constexpr std::uint8_t value = 0xA0;
      static Parse_info method(const uint8_t *bytes, std::size_t length);
    };

    struct ControlChange : NotInstantiable
    {
      static constexpr std::uint8_t value = 0xB0;
      static Parse_info method(const uint8_t *bytes, std::size_t length);
    };

    struct ProgramChange : NotInstantiable
    {
      static constexpr std::uint8_t value = 0xC0;
      static Parse_info method(const uint8_t *bytes, std::size_t length);
    };

    struct ChannelPressure : NotInstantiable
    {
      static constexpr std::uint8_t value = 0xD0;
      static Parse_info method(const uint8_t *bytes, std::size_t length);
    };

    struct PitchBend : NotInstantiable
    {
      static constexpr std::uint8_t value = 0xE0;
      static Parse_info method(const uint8_t *bytes, std::size_t length);
    };

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
            static Parse_info method(const uint8_t *, std::size_t);
          };
          struct SampleDataPacket : NotInstantiable
          {
            static constexpr uint8_t value = 0x02;
            static Parse_info method(const uint8_t *, std::size_t);
          };
          struct SampleDumpRequest : NotInstantiable
          {
            static constexpr uint8_t value = 0x03;
            static Parse_info method(const uint8_t *, std::size_t);
          };
          struct MidiTimeCode : NotInstantiable
          {
            static constexpr uint8_t value = 0x04;
            static Parse_info method(const uint8_t *, std::size_t);
          };
          struct SampleDumpExtensions : NotInstantiable
          {
            static constexpr uint8_t value = 0x05;
            static Parse_info method(const uint8_t *, std::size_t);
          };
          struct GeneralInformation : NotInstantiable
          {
            static constexpr uint8_t value = 0x06;
            static Parse_info method(const uint8_t *, std::size_t);
          };
          struct FileDump : NotInstantiable
          {
            static constexpr uint8_t value = 0x07;
            static Parse_info method(const uint8_t *, std::size_t);
          };
          struct MidiTuningStandard : NotInstantiable
          {
            static constexpr uint8_t value = 0x08;
            static Parse_info method(const uint8_t *, std::size_t);
          };
          struct GeneralMidi : NotInstantiable
          {
            static constexpr uint8_t value = 0x09;
            static Parse_info method(const uint8_t *, std::size_t);
          };
          struct EndOfFile : NotInstantiable
          {
            static constexpr uint8_t value = 0x7B;
            static Parse_info method(const uint8_t *, std::size_t);
          };
          struct Wait : NotInstantiable
          {
            static constexpr uint8_t value = 0x7C;
            static Parse_info method(const uint8_t *, std::size_t);
          };
          struct Cancel : NotInstantiable
          {
            static constexpr uint8_t value = 0x7D;
            static Parse_info method(const uint8_t *, std::size_t);
          };
          struct NAK : NotInstantiable
          {
            static constexpr uint8_t value = 0x7E;
            static Parse_info method(const uint8_t *, std::size_t);
          };
          struct ACK : NotInstantiable
          {
            static constexpr uint8_t value = 0x7F;
            static Parse_info method(const uint8_t *, std::size_t);
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
          static constexpr auto method = pgm::Process<Parse_info(const uint8_t *&, std::size_t &)>{}
            << StripBytes<1>
            << CheckLength<2>
            << SwitcherMaker::Interpreter<Parse_info(const uint8_t *, std::size_t), CaseList>(
              GetByteMask<1, 0xFF>,
              [](auto...) { return Parse_info{PARSE_STATUS::ERROR, "Invalid case"}; });
        };

        struct UniRT : NotInstantiable
        {
          struct MidiTimeCode : NotInstantiable
          {
            static constexpr uint8_t value = 0x01;
            static Parse_info method(const uint8_t *, std::size_t);
          };
          struct ShowControls : NotInstantiable
          {
            static constexpr uint8_t value = 0x02;
            static Parse_info method(const uint8_t *, std::size_t);
          };
          struct NotationInfo : NotInstantiable
          {
            static constexpr uint8_t value = 0x03;
            static Parse_info method(const uint8_t *, std::size_t);
          };
          struct DeviceControl : NotInstantiable
          {
            static constexpr uint8_t value = 0x04;
            static Parse_info method(const uint8_t *, std::size_t);
          };
          struct RTMTCCue : NotInstantiable
          {
            static constexpr uint8_t value = 0x05;
            static Parse_info method(const uint8_t *, std::size_t);
          };
          struct MMCCommands : NotInstantiable
          {
            static constexpr uint8_t value = 0x06;
            static Parse_info method(const uint8_t *, std::size_t);
          };
          struct MMCResponse : NotInstantiable
          {
            static constexpr uint8_t value = 0x07;
            static Parse_info method(const uint8_t *, std::size_t);
          };
          struct MidiTuning : NotInstantiable
          {
            static constexpr uint8_t value = 0x08;
            static Parse_info method(const uint8_t *, std::size_t);
          };

        private:
          using CaseList = std::tuple<
            MidiTimeCode,
            ShowControls,
            NotationInfo,
            DeviceControl,
            RTMTCCue,
            MMCCommands,
            MMCResponse,
            MidiTuning>;

        public:
          static constexpr uint8_t value = 0x7F;
          static constexpr auto method = pgm::Process<Parse_info(const uint8_t *&, std::size_t &)>{}
            << StripBytes<1>
            << CheckLength<2>
            << SwitcherMaker::Interpreter<Parse_info(const uint8_t *, std::size_t), CaseList>(
              GetByteMask<1, 0xFF>,
              [](auto...) { return Parse_info{PARSE_STATUS::ERROR, "Invalid case"}; });
        };

      private:
        using CaseList = std::tuple<
          UniNonRT,
          UniRT>;

      public:
        // @return false if there is a match else true
        static Parse_info specificMatchFunction(const uint8_t *&, std::size_t &);
        static Parse_info interpretSpecificSysEx(const uint8_t *, std::size_t lenght);
        static constexpr uint8_t value = 0xF0;
        static constexpr auto method =
          pgm::Process<Parse_info(const uint8_t *&, std::size_t &)>{}
          << StripStatus
          << CheckLength<1>
          << SwitcherMaker::Interpreter<Parse_info(const uint8_t *, std::size_t), CaseList>(
            GetFirstByte,
            pgm::Process<Parse_info(const uint8_t *&, std::size_t &)>{} << specificMatchFunction << interpretSpecificSysEx);
      };
      struct MTC : NotInstantiable
      {
        static constexpr uint8_t value = 0xF1;
        static Parse_info method(const uint8_t *, std::size_t);
      };
      struct Songpos : NotInstantiable
      {
        static constexpr uint8_t value = 0xF2;
        static Parse_info method(const uint8_t *, std::size_t);
      };
      struct SongSel : NotInstantiable
      {
        static constexpr uint8_t value = 0xF3;
        static Parse_info method(const uint8_t *, std::size_t);
      };
      struct Tune : NotInstantiable
      {
        static constexpr uint8_t value = 0xF6;
        static Parse_info method(const uint8_t *, std::size_t);
      };
      // System Real Time
      struct Clock : NotInstantiable
      {
        static constexpr uint8_t value = 0xF8;
        static Parse_info method(const uint8_t *, std::size_t);
      };
      struct Start : NotInstantiable
      {
        static constexpr uint8_t value = 0xFA;
        static Parse_info method(const uint8_t *, std::size_t);
      };
      struct Continue : NotInstantiable
      {
        static constexpr uint8_t value = 0xFB;
        static Parse_info method(const uint8_t *, std::size_t);
      };
      struct Stop : NotInstantiable
      {
        static constexpr uint8_t value = 0xFC;
        static Parse_info method(const uint8_t *, std::size_t);
      };
      struct ActiveSensing : NotInstantiable
      {
        static constexpr uint8_t value = 0xFE;
        static Parse_info method(const uint8_t *, std::size_t);
      };
      struct SystemReset : NotInstantiable
      {
        static constexpr uint8_t value = 0xFF;
        static Parse_info method(const uint8_t *, std::size_t);
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
      static constexpr auto method = pgm::Process<Parse_info(const uint8_t *&, std::size_t &)>{}
        << SwitcherMaker::Interpreter<Parse_info(const uint8_t *, std::size_t), CaseList>(
          GetFirstByte,
          [](auto...) { return Parse_info{PARSE_STATUS::ERROR, "Invalid case"}; });
    };

  private:
    using CaseList = std::tuple<
      NoteOff,
      NoteOn,
      PolyPressure,
      ControlChange,
      ProgramChange,
      ChannelPressure,
      PitchBend,
      SystemMessage>;

  public:
    static constexpr std::uint8_t value = 0x80;
    static constexpr auto method = SwitcherMaker::Interpreter<Parse_info(const uint8_t *, std::size_t), CaseList>(
      GetFirstByteMask<0xF0>,
      [](auto...) { return Parse_info{PARSE_STATUS::ERROR, "Invalid case"}; });
  };

  struct M2 : NotInstantiable
  {
  public:
    static constexpr std::uint8_t value = 0x00;
    static constexpr auto method = [](auto...) {return Parse_info{PARSE_STATUS::ERROR, "MIDI2 not impl"};};
  };

private:
  using CaseList = std::tuple<
    M1,
    M2>;

public:
  static constexpr auto Interpret = pgm::Process<Parse_info(const uint8_t *&, std::size_t &)>{}
    << CheckLength<1>
    << SwitcherMaker::Interpreter<Parse_info(const uint8_t *, std::size_t), CaseList>(
      GetFirstByteMask<0x80>,
      [](auto...) { return Parse_info{PARSE_STATUS::ERROR, "Invalid case"}; });
};

////////////////////////////////////////////////////////////////////////////////////

Parse_info analyzer(const uint8_t *bytes, std::size_t length)
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

Parse_info MidiBytes::M1::NoteOn::method(const uint8_t *bytes, std::size_t length)
{
  if (!HasLength<3>(bytes, length))
    return {PARSE_STATUS::ERROR, "Length != 3"};
  prtrem("NoteOn", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::NoteOff::method(const uint8_t *bytes, std::size_t length)
{
  if (!HasLength<3>(bytes, length))
    return {PARSE_STATUS::ERROR, "Length != 3"};
  prtrem("NoteOff", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::PolyPressure::method(const uint8_t *bytes, std::size_t length)
{
  if (!HasLength<3>(bytes, length))
    return {PARSE_STATUS::ERROR, "Length != 3"};
  prtrem("PolyPressure", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::ControlChange::method(const uint8_t *bytes, std::size_t length)
{
  if (!HasLength<3>(bytes, length))
    return {PARSE_STATUS::ERROR, "Length != 3"};
  prtrem("ControlChange", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::ProgramChange::method(const uint8_t *bytes, std::size_t length)
{
  if (!HasLength<2>(bytes, length))
    return {PARSE_STATUS::ERROR, "Length != 2"};
  prtrem("ProgramChange", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::ChannelPressure::method(const uint8_t *bytes, std::size_t length)
{
  if (!HasLength<2>(bytes, length))
    return {PARSE_STATUS::ERROR, "Length != 2"};
  prtrem("ChannelPressure", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::PitchBend::method(const uint8_t *bytes, std::size_t length)
{
  if (!HasLength<3>(bytes, length))
    return {PARSE_STATUS::ERROR, "Length != 3"};
  prtrem("PitchBend", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::SysEx::UniNonRT::SampleDumpHeader::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("SampleDumpHeader", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::SysEx::UniNonRT::SampleDataPacket::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("SampleDataPacket", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::SysEx::UniNonRT::SampleDumpRequest::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("SampleDumpRequest", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::SysEx::UniNonRT::MidiTimeCode::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("NRT_MidiTimeCode", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::SysEx::UniNonRT::SampleDumpExtensions::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("SampleDumpExtensions", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::SysEx::UniNonRT::GeneralInformation::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("GeneralInformation", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::SysEx::UniNonRT::FileDump::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("FileDump", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::SysEx::UniNonRT::MidiTuningStandard::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("MidiTuningStandard", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::SysEx::UniNonRT::GeneralMidi::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("GeneralMidi", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::SysEx::UniNonRT::EndOfFile::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("EndOfFile", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::SysEx::UniNonRT::Wait::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("Wait", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::SysEx::UniNonRT::Cancel::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("Cancel", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::SysEx::UniNonRT::NAK::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("NAK", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::SysEx::UniNonRT::ACK::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("ACK", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::SysEx::UniRT::MidiTimeCode::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("RT_MidiTimeCode", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::SysEx::UniRT::ShowControls::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("ShowControls", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::SysEx::UniRT::NotationInfo::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("NotationInfo", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::SysEx::UniRT::DeviceControl::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("DeviceControl", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::SysEx::UniRT::RTMTCCue::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("RTMTCCue", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::SysEx::UniRT::MMCCommands::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("MMCCommands", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::SysEx::UniRT::MMCResponse::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("MMCResponse", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::SysEx::UniRT::MidiTuning::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("MidiTuning", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::SysEx::specificMatchFunction(const uint8_t *&bytes, std::size_t &length)
{
  prtrem("specificMatchFunction", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::SysEx::interpretSpecificSysEx(const uint8_t *bytes, std::size_t length)
{
  prtrem("interpretSpecificSysEx", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::MTC::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("MTC", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::Songpos::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("Songpos", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::SongSel::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("SongSel", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::Tune::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("Tune", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::Clock::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("Clock", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::Start::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("Start", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::Continue::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("Continue", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::Stop::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("Stop", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::ActiveSensing::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("ActiveSensing", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}
Parse_info MidiBytes::M1::SystemMessage::SystemReset::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("SystemReset", bytes, length);
  return {PARSE_STATUS::SUCCESS};
}

int main()
{
  // uint8_t bytes[] = {0xF0, 0x7E, 0x69, 0x07, 0x03, 0x42, 'B', 'I', 'N', ' ', 'e', 'r', 'a', 'e', '_', 't', 'o', 'u', 'c', 'h', '_', 's', 'o', 'f', 't', 0xF7};
  uint8_t bytes[] = {0xF0, 0x7E, 0x7F, 0x06, 0x01, 0xF7};
  // uint8_t bytes[] = {0x94, 0x40, 0x7F};
  // uint8_t bytes[] = {0xF0, 0x7F, 0x69, 0x04, 0x01, 0x00, 0x00, 0xF7};
  Parse_info ret = MidiBytes::Interpret(bytes, sizeof(bytes));
  switch (ret.status())
  {
  case PARSE_STATUS::UNDEFINED:
    printf("Undefined return status\n");
    break;
  case PARSE_STATUS::SUCCESS:
    printf("Message parsed successfully!\n");
    break;
  case PARSE_STATUS::ERROR:
    printf("Error: %s\n", ret.description());
  }
  return 0;
}

/*/

#include <optional>

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

constexpr auto mySwitcher = SwitcherMaker::Interpreter<std::optional<int>(const uint8_t *, std::size_t), CaseList>(
  mycond,
  mydefault);

constexpr auto p = pgm::Process<std::optional<int>(const uint8_t *&, std::size_t &)>{}
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
