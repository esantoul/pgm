#ifndef MIDI_PARSER_HPP
#define MIDI_PARSER_HPP

#include "optional_w.hpp"
#include "../include/pgm.hpp"
#include <cstddef>
#include <cstdint>

// Switcher utility function for MIDI interpretation
class Interpreter
{
public:
  template <typename Proto_t, typename CaseTuple_t, typename CondFun_t, typename Default_t>
  static constexpr auto FromCaseTuple(CondFun_t cf, Default_t df)
  {
    return FromCaseTupleHelper<Proto_t, CaseTuple_t>(cf, df, std::make_index_sequence<std::tuple_size_v<CaseTuple_t>>());
  }

  // template <typename Proto_t, typename CaseTuple_t, typename CondFun_t>
  // static constexpr auto FromCaseTuple(CondFun_t cf)
  // {
  //   return FromCaseTupleHelper<Proto_t, CaseTuple_t>(
  //     cf, [](auto...) -> pgm::Helper::ret_t<Proto_t> { return {}; }, std::make_index_sequence<std::tuple_size_v<CaseTuple_t>>());
  // }

private:
  template <typename Proto_t, typename CaseTuple_t, typename CondFun_t, typename Default_t, std::size_t... case_idx>
  static constexpr auto FromCaseTupleHelper(CondFun_t cf, Default_t df, std::index_sequence<case_idx...>)
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
  UNIMPLEMENTED,
  ERROR_UNKNOWN,
  ERROR_MSG_TOO_SHORT,
  ERROR_UNADEQUATE_LENGTH,
  ERROR_INVALID_CASE,
};

using PInfo = info<PARSE_STATUS>;

// Tasks Definitions
template <std::size_t len>
constexpr auto CheckLength = [](const std::uint8_t *, std::size_t length) -> PInfo {
  return length < len ? PInfo{PInfo::E::ERROR_MSG_TOO_SHORT} : PInfo{};
};

template <std::size_t len>
constexpr auto HasLength = [](const std::uint8_t *, std::size_t length) -> bool {
  return length == len;
};

template <std::size_t len>
constexpr auto StripBytes = [](const std::uint8_t *&bytes, std::size_t &length) -> PInfo {
  bytes = length > len ? bytes + len : bytes + length;
  length = length > len ? length - len : 0;
  return {};
};

constexpr auto StripStatus = [](const std::uint8_t *&bytes, std::size_t &length) -> PInfo {
  if (length < 2)
    return {PInfo::E::ERROR_MSG_TOO_SHORT};
  ++bytes, length -= 2;
  return {};
};

template <std::size_t idx, std::uint8_t mask>
constexpr auto GetByteMask = [](const std::uint8_t *bytes, std::size_t) -> std::uint8_t {
  return bytes[idx] & mask;
};

template <std::uint8_t mask>
constexpr auto GetFirstByteMask = GetByteMask<0, mask>;

constexpr auto GetFirstByte = GetFirstByteMask<0xFF>;

constexpr auto InvalidCase = [](auto...) { return PInfo{PInfo::E::ERROR_INVALID_CASE}; };

constexpr auto Unimplemented = [](auto ...) { return PInfo{PInfo::E::UNIMPLEMENTED}; };

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
      static PInfo method(const std::uint8_t *, std::size_t);
    };

    struct NoteOn : NotInstantiable
    {
      static constexpr std::uint8_t value = 0x90;
      static PInfo method(const std::uint8_t *, std::size_t);
    };

    struct PolyPressure : NotInstantiable
    {
      static constexpr std::uint8_t value = 0xA0;
      static PInfo method(const std::uint8_t *, std::size_t);
    };

    struct ControlChange : NotInstantiable
    {
      static constexpr std::uint8_t value = 0xB0;
      static PInfo method(const std::uint8_t *, std::size_t);
    };

    struct ProgramChange : NotInstantiable
    {
      static constexpr std::uint8_t value = 0xC0;
      static PInfo method(const std::uint8_t *, std::size_t);
    };

    struct ChannelPressure : NotInstantiable
    {
      static constexpr std::uint8_t value = 0xD0;
      static PInfo method(const std::uint8_t *, std::size_t);
    };

    struct PitchBend : NotInstantiable
    {
      static constexpr std::uint8_t value = 0xE0;
      static PInfo method(const std::uint8_t *, std::size_t);
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
            static constexpr std::uint8_t value = 0x01;
            static PInfo method(const std::uint8_t *, std::size_t);
          };
          struct SampleDataPacket : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x02;
            static PInfo method(const std::uint8_t *, std::size_t);
          };
          struct SampleDumpRequest : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x03;
            static PInfo method(const std::uint8_t *, std::size_t);
          };
          struct MidiTimeCode : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x04;
            static PInfo method(const std::uint8_t *, std::size_t);
          };
          struct SampleDumpExtensions : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x05;
            static PInfo method(const std::uint8_t *, std::size_t);
          };
          struct GeneralInformation : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x06;
            static PInfo method(const std::uint8_t *, std::size_t);
          };
          struct FileDump : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x07;
            static PInfo method(const std::uint8_t *, std::size_t);
          };
          struct MidiTuningStandard : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x08;
            static PInfo method(const std::uint8_t *, std::size_t);
          };
          struct GeneralMidi : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x09;
            static PInfo method(const std::uint8_t *, std::size_t);
          };
          struct EndOfFile : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x7B;
            static PInfo method(const std::uint8_t *, std::size_t);
          };
          struct Wait : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x7C;
            static PInfo method(const std::uint8_t *, std::size_t);
          };
          struct Cancel : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x7D;
            static PInfo method(const std::uint8_t *, std::size_t);
          };
          struct NAK : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x7E;
            static PInfo method(const std::uint8_t *, std::size_t);
          };
          struct ACK : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x7F;
            static PInfo method(const std::uint8_t *, std::size_t);
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
          static constexpr std::uint8_t value = 0x7E;
          static constexpr auto method = pgm::Process<PInfo(const std::uint8_t *&, std::size_t &)>{}
            << StripBytes<1>
            << CheckLength<2>
            << Interpreter::FromCaseTuple<PInfo(const std::uint8_t *, std::size_t), CaseList>(
              GetByteMask<1, 0xFF>,
              InvalidCase);
        };

        struct UniRT : NotInstantiable
        {
          struct MidiTimeCode : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x01;
            static PInfo method(const std::uint8_t *, std::size_t);
          };
          struct ShowControls : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x02;
            static PInfo method(const std::uint8_t *, std::size_t);
          };
          struct NotationInfo : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x03;
            static PInfo method(const std::uint8_t *, std::size_t);
          };
          struct DeviceControl : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x04;
            static PInfo method(const std::uint8_t *, std::size_t);
          };
          struct RTMTCCue : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x05;
            static PInfo method(const std::uint8_t *, std::size_t);
          };
          struct MMCCommands : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x06;
            static PInfo method(const std::uint8_t *, std::size_t);
          };
          struct MMCResponse : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x07;
            static PInfo method(const std::uint8_t *, std::size_t);
          };
          struct MidiTuning : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x08;
            static PInfo method(const std::uint8_t *, std::size_t);
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
          static constexpr std::uint8_t value = 0x7F;
          static constexpr auto method = pgm::Process<PInfo(const std::uint8_t *&, std::size_t &)>{}
            << StripBytes<1>
            << CheckLength<2>
            << Interpreter::FromCaseTuple<PInfo(const std::uint8_t *, std::size_t), CaseList>(
              GetByteMask<1, 0xFF>,
              InvalidCase);
        };

      private:
        using CaseList = std::tuple<
          UniNonRT,
          UniRT>;

      public:
        // @return false if there is a match else true
        static PInfo specificMatchFunction(const std::uint8_t *&, std::size_t &);
        static PInfo interpretSpecificSysEx(const std::uint8_t *, std::size_t);

        static constexpr std::uint8_t value = 0xF0;
        static constexpr auto method =
          pgm::Process<PInfo(const std::uint8_t *&, std::size_t &)>{}
          << StripStatus
          << CheckLength<1>
          << Interpreter::FromCaseTuple<PInfo(const std::uint8_t *, std::size_t), CaseList>(
            GetFirstByte,
            pgm::Process<PInfo(const std::uint8_t *&, std::size_t &)>{} << specificMatchFunction << interpretSpecificSysEx);
      };
      struct MTC : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xF1;
        static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct Songpos : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xF2;
        static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct SongSel : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xF3;
        static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct TuneRequest : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xF6;
        static PInfo method(const std::uint8_t *, std::size_t);
      };
      // System Real Time
      struct TimingClock : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xF8;
        static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct Start : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xFA;
        static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct Continue : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xFB;
        static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct Stop : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xFC;
        static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct ActiveSensing : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xFE;
        static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct SystemReset : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xFF;
        static PInfo method(const std::uint8_t *, std::size_t);
      };

    private:
      using CaseList = std::tuple<
        SysEx,
        MTC,
        Songpos,
        SongSel,
        TuneRequest,
        TimingClock,
        Start,
        Continue,
        Stop,
        ActiveSensing,
        SystemReset>;

    public:
      static constexpr std::uint8_t value = 0xF0;
      static constexpr auto method = pgm::Process<PInfo(const std::uint8_t *&, std::size_t &)>{}
        << Interpreter::FromCaseTuple<PInfo(const std::uint8_t *, std::size_t), CaseList>(
          GetFirstByte,
          InvalidCase);
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
    static constexpr auto method = Interpreter::FromCaseTuple<PInfo(const std::uint8_t *, std::size_t), CaseList>(
      GetFirstByteMask<0xF0>,
      InvalidCase);
  };

  struct M2 : NotInstantiable
  {
  public:
    struct Utility : NotInstantiable
    {
      struct NOOP : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x00;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct JRClock : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x10;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct JRTimestamp : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x20;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };

    private:
      using CaseList = std::tuple<
        NOOP,
        JRClock,
        JRTimestamp>;

    public:
      static constexpr std::uint8_t value = 0x00;
      static constexpr auto method = Interpreter::FromCaseTuple<PInfo(const std::uint8_t *, std::size_t), CaseList>(
        GetByteMask<1, 0xF0>,
        InvalidCase);
    };
    struct System : NotInstantiable
    {
      struct MTC : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xF1;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct SongPos : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xF2;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct SongSel : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xF3;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct TuneRequest : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xF6;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct TimingClock : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xF8;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct Start : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xFA;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct Continue : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xFB;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct Stop : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xFC;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct ActiveSensing : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xFE;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct Reset : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xFF;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };

    private:
      using CaseList = std::tuple<
        MTC,
        SongPos,
        SongSel,
        TuneRequest,
        TimingClock,
        Start,
        Continue,
        Stop,
        ActiveSensing,
        Reset>;

    public:
      static constexpr std::uint8_t value = 0x10;
      static constexpr auto method = Interpreter::FromCaseTuple<PInfo(const std::uint8_t *, std::size_t), CaseList>(
        GetByteMask<1, 0xFF>,
        InvalidCase);
    };
    struct Midi1Channel : NotInstantiable
    {
      struct NoteOff : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x80;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct NoteOn : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x90;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct PolyPressure : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xA0;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct ControlChange : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xB0;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct ProgramChange : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xC0;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct ChannelPressure : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xD0;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct PitchBend : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xE0;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };

    private:
      using CaseList = std::tuple<
        NoteOff,
        NoteOn,
        PolyPressure,
        ControlChange,
        ProgramChange,
        ChannelPressure,
        PitchBend>;

    public:
      static constexpr std::uint8_t value = 0x20;
      static constexpr auto method = Interpreter::FromCaseTuple<PInfo(const std::uint8_t *, std::size_t), CaseList>(
        GetByteMask<1, 0xF0>,
        InvalidCase);
    };
    struct Data64Bits : NotInstantiable
    {
      struct SysEx1Packet : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x00;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct SysExStart : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x10;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct SysExContinue : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x20;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct SysExEnd : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x30;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };

    private:
      using CaseList = std::tuple<
        SysEx1Packet,
        SysExStart,
        SysExContinue,
        SysExEnd>;

    public:
      static constexpr std::uint8_t value = 0x30;
      static constexpr auto method = pgm::Process<PInfo(const std::uint8_t *, std::size_t)>{}
        << CheckLength<8>
        << Interpreter::FromCaseTuple<PInfo(const std::uint8_t *, std::size_t), CaseList>(
          GetByteMask<1, 0xF0>,
          InvalidCase);
    };
    struct Midi2Channel : NotInstantiable
    {
      struct RegistPerNoteCtrl : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x00;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct AssignPerNoteCtrl : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x10;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct RegistCtrl : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x20;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct AssignCtrl : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x30;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct RelativeRegistCtrl : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x40;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct RelativeAssignCtrl : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x60;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct PerNotePitchBend : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x70;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct NoteOff : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x80;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct NoteOn : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x90;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct PolyPressure : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xA0;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct ControlChange : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xB0;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct ProgramChange : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xC0;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct ChannelPressure : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xD0;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct PitchBend : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xE0;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct PerNoteManagement : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xF0;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
    private:
      using CaseList = std::tuple<
        RegistPerNoteCtrl,
        AssignPerNoteCtrl,
        RegistCtrl,
        AssignCtrl,
        RelativeRegistCtrl,
        RelativeAssignCtrl,
        PerNotePitchBend,
        NoteOff,
        NoteOn,
        PolyPressure,
        ControlChange,
        ProgramChange,
        ChannelPressure,
        PitchBend,
        PerNoteManagement>;

    public:
      static constexpr std::uint8_t value = 0x40;
      static constexpr auto method = pgm::Process<PInfo(const std::uint8_t *, std::size_t)>{}
        << CheckLength<8>
        << Interpreter::FromCaseTuple<PInfo(const std::uint8_t *, std::size_t), CaseList>(
          GetByteMask<1, 0xF0>,
          InvalidCase);
    };
    struct Data128Bits : NotInstantiable
    {
      struct SysEx8In1Packet : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x00;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct SysEx8Start : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x10;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct SysEx8Continue : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x20;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct SysEx8End : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x30;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct MixedDataSetHeader : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x80;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };
      struct MixedDataSetPayload : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x90;
        static constexpr auto method = Unimplemented; // static PInfo method(const std::uint8_t *, std::size_t);
      };

    private:
      using CaseList = std::tuple<
        SysEx8In1Packet,
        SysEx8Start,
        SysEx8Continue,
        SysEx8End,
        MixedDataSetHeader,
        MixedDataSetPayload>;

    public:
      static constexpr std::uint8_t value = 0x50;
      static constexpr auto method = pgm::Process<PInfo(const std::uint8_t *, std::size_t)>{}
        << CheckLength<16>
        << Interpreter::FromCaseTuple<PInfo(const std::uint8_t *, std::size_t), CaseList>(
          GetByteMask<1, 0xF0>,
          InvalidCase);
    };

  private:
    using CaseList = std::tuple<
      Utility,
      System,
      Midi1Channel,
      Data64Bits,
      Midi2Channel,
      Data128Bits>;

  public:
    static constexpr auto checkMidi2Status = Unimplemented; // static PInfo checkMidi2Status(const std::uint8_t *, std::size_t);

    static constexpr std::uint8_t value = 0x00;
    static constexpr auto method = pgm::Process<PInfo(const std::uint8_t *&, std::size_t &)>{}
      << checkMidi2Status
      << CheckLength<4>
      << Interpreter::FromCaseTuple<PInfo(const std::uint8_t *, std::size_t), CaseList>(
        GetFirstByteMask<0xF0>,
        InvalidCase);
  };

private:
  using CaseList = std::tuple<
    M1,
    M2>;

public:
  [[maybe_unused]] static constexpr auto Interpret = pgm::Process<PInfo(const std::uint8_t *&, std::size_t &)>{}
    << CheckLength<1>
    << Interpreter::FromCaseTuple<PInfo(const std::uint8_t *, std::size_t), CaseList>(
      GetFirstByteMask<0x80>,
      InvalidCase);
};

#endif // MIDI_PARSER_HPP