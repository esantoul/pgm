#ifndef MIDI_PARSER_HPP
#define MIDI_PARSER_HPP

/**
 * @file midi_parser.hpp
 * @author Etienne SANTOUL github.com/esantoul
 * @brief
 * @version 0.1
 * @date 2021-02-04
 *
 * @copyright BSD 2-Clause License. Copyright (c) 2021, Etienne SANTOUL. All rights reserved.
 */

#include "info_types.hpp"
#include "../include/pgm.hpp"
#include <cstddef>
#include <cstdint>

 // Switcher utility function for MIDI interpretation
class SwitcherFactory
{
public:
  template <typename Proto_t, typename CaseTuple_t, typename CondFun_t, typename Default_t>
  static constexpr auto Parse(CondFun_t cf, Default_t df)
  {
    return ParseHelper<Proto_t, CaseTuple_t>(cf, df, std::make_index_sequence<std::tuple_size_v<CaseTuple_t>>());
  }

  template <typename Proto_t, typename CaseTuple_t, typename CondFun_t, typename Default_t>
  static constexpr auto Size(CondFun_t cf, Default_t df)
  {
    return SizeHelper<Proto_t, CaseTuple_t>(cf, df, std::make_index_sequence<std::tuple_size_v<CaseTuple_t>>());
  }

private:
  template <typename Proto_t, typename CaseTuple_t, typename CondFun_t, typename Default_t, std::size_t... case_idx>
  static constexpr auto ParseHelper(CondFun_t cf, Default_t df, std::index_sequence<case_idx...>)
  {
    return pgm::Switcher{
        pgm::Prototype<Proto_t>{},
        cf,
        df,
        std::make_pair(
            std::tuple_element_t<case_idx, CaseTuple_t>::value,
            std::tuple_element_t<case_idx, CaseTuple_t>::method)...};
  }

  template <typename Proto_t, typename CaseTuple_t, typename CondFun_t, typename Default_t, std::size_t... case_idx>
  static constexpr auto SizeHelper(CondFun_t cf, Default_t df, std::index_sequence<case_idx...>)
  {
    return pgm::Switcher{
        pgm::Prototype<Proto_t>{},
        cf,
        df,
        std::make_pair(
            std::tuple_element_t<case_idx, CaseTuple_t>::value,
            std::tuple_element_t<case_idx, CaseTuple_t>::insight)...};
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

using ParseInfo = utils::info<PARSE_STATUS>;

class MidiSize
{
public:
  enum class Status
  {
    Set,
    SysEx,
    Discard,
    Unknown,
    Error
  };

  constexpr MidiSize() = default;

  template <std::uint8_t val, typename = std::enable_if_t<(val != 0) && (val <= 16)>>
  static constexpr MidiSize Match() { return {val}; }

  static constexpr MidiSize Syx() { return {0xF7}; }

  template <std::uint8_t val, typename = std::enable_if_t<(val >= 1 && val <= 16)>>
  static constexpr MidiSize Discard() { return {0x80 + val}; }

  static constexpr MidiSize Unknown() { return {}; }

  constexpr operator bool() const { return mVal; }

  constexpr uint8_t value() const { return mVal & 0x1F; }

  constexpr Status status() const
  {
    if (mVal != 0 && mVal <= 16)
      return Status::Set;
    else if (mVal >= 0x81 && mVal <= 0x90)
      return Status::Discard;
    switch (mVal)
    {
    case 0x00:
      return Status::Error;
    case 0xF7:
      return Status::SysEx;
    }
    return Status::Error;
  }

private:
  constexpr MidiSize(std::uint8_t v) : mVal{v} {}

  std::uint8_t mVal{};
};

// Parse Frequent Tasks
template <std::size_t len>
constexpr auto CheckLength = [](const std::uint8_t *, std::size_t length) -> ParseInfo {
  return length < len ? ParseInfo{ParseInfo::E::ERROR_MSG_TOO_SHORT} : ParseInfo{};
};

template <std::size_t len>
constexpr auto HasLength = [](const std::uint8_t *, std::size_t length) -> bool {
  return length == len;
};

template <std::size_t len>
constexpr auto StripBytes = [](const std::uint8_t *&bytes, std::size_t &length) -> ParseInfo {
  bytes = length > len ? bytes + len : bytes + length;
  length = length > len ? length - len : 0;
  return {};
};

constexpr auto StripStatus = [](const std::uint8_t *&bytes, std::size_t &length) -> ParseInfo {
  if (length < 2)
    return {ParseInfo::E::ERROR_MSG_TOO_SHORT};
  ++bytes, length -= 2;
  return {};
};

constexpr auto InvalidParse = [](auto...) { return ParseInfo{ParseInfo::E::ERROR_INVALID_CASE}; };

constexpr auto Unimplemented = [](auto ...) { return ParseInfo{ParseInfo::E::UNIMPLEMENTED}; };

// Parse Conditions
template <std::size_t idx, std::uint8_t mask>
constexpr auto GetByteMask = [](const std::uint8_t *bytes, std::size_t) -> std::uint8_t {
  return bytes[idx] & mask;
};

template <std::uint8_t mask>
constexpr auto GetFirstByteMask = GetByteMask<0, mask>;

constexpr auto GetFirstByte = GetFirstByteMask<0xFF>;

// Insight Frequent Tasks

constexpr auto InvalidInsight = [](auto...) { return MidiSize::Unknown(); };

// Insight conditions

constexpr auto u8Forward = [](const std::uint8_t &val) -> const std::uint8_t & { return val; };

template <std::uint8_t mask>
constexpr auto u8Mask = [](const std::uint8_t &val) -> std::uint8_t { return val & mask; };

struct NotInstantiable
{
private:
  constexpr NotInstantiable() {}
};

// Midi Bytes entire definition
struct MidiBytes : NotInstantiable
{
  struct M1 : NotInstantiable
  {
    struct NoteOff : NotInstantiable
    {
      static constexpr std::uint8_t value = 0x80;
      static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      static constexpr auto insight = [](auto...) { return MidiSize::Match<3>(); };
    };

    struct NoteOn : NotInstantiable
    {
      static constexpr std::uint8_t value = 0x90;
      static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      static constexpr auto insight = [](auto...) { return MidiSize::Match<3>(); };
    };

    struct PolyPressure : NotInstantiable
    {
      static constexpr std::uint8_t value = 0xA0;
      static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      static constexpr auto insight = [](auto...) { return MidiSize::Match<3>(); };
    };

    struct ControlChange : NotInstantiable
    {
      static constexpr std::uint8_t value = 0xB0;
      static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      static constexpr auto insight = [](auto...) { return MidiSize::Match<3>(); };
    };

    struct ProgramChange : NotInstantiable
    {
      static constexpr std::uint8_t value = 0xC0;
      static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      static constexpr auto insight = [](auto...) { return MidiSize::Match<2>(); };
    };

    struct ChannelPressure : NotInstantiable
    {
      static constexpr std::uint8_t value = 0xD0;
      static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      static constexpr auto insight = [](auto...) { return MidiSize::Match<2>(); };
    };

    struct PitchBend : NotInstantiable
    {
      static constexpr std::uint8_t value = 0xE0;
      static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      static constexpr auto insight = [](auto...) { return MidiSize::Match<3>(); };
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
            static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
          };
          struct SampleDataPacket : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x02;
            static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
          };
          struct SampleDumpRequest : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x03;
            static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
          };
          struct MidiTimeCode : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x04;
            static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
          };
          struct SampleDumpExtensions : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x05;
            static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
          };
          struct GeneralInformation : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x06;
            static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
          };
          struct FileDump : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x07;
            static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
          };
          struct MidiTuningStandard : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x08;
            static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
          };
          struct GeneralMidi : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x09;
            static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
          };
          struct EndOfFile : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x7B;
            static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
          };
          struct Wait : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x7C;
            static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
          };
          struct Cancel : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x7D;
            static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
          };
          struct NAK : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x7E;
            static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
          };
          struct ACK : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x7F;
            static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
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
          static constexpr auto method = pgm::Process<ParseInfo(const std::uint8_t *&, std::size_t &)>{}
            << StripBytes<1>
            << CheckLength<2>
            << SwitcherFactory::Parse<ParseInfo(const std::uint8_t *, std::size_t), CaseList>(
              GetByteMask<1, 0xFF>,
              InvalidParse);
        };

        struct UniRT : NotInstantiable
        {
          struct MidiTimeCode : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x01;
            static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
          };
          struct ShowControls : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x02;
            static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
          };
          struct NotationInfo : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x03;
            static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
          };
          struct DeviceControl : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x04;
            static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
          };
          struct RTMTCCue : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x05;
            static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
          };
          struct MMCCommands : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x06;
            static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
          };
          struct MMCResponse : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x07;
            static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
          };
          struct MidiTuning : NotInstantiable
          {
            static constexpr std::uint8_t value = 0x08;
            static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
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
          static constexpr auto method = pgm::Process<ParseInfo(const std::uint8_t *&, std::size_t &)>{}
            << StripBytes<1>
            << CheckLength<2>
            << SwitcherFactory::Parse<ParseInfo(const std::uint8_t *, std::size_t), CaseList>(
              GetByteMask<1, 0xFF>,
              InvalidParse);
        };

      private:
        using CaseList = std::tuple<
          UniNonRT,
          UniRT>;

      public:
        // @return false if there is a match else true
        static constexpr ParseInfo specificMatchFunction(const std::uint8_t *&, std::size_t &);
        static constexpr ParseInfo interpretSpecificSysEx(const std::uint8_t *, std::size_t);

        static constexpr std::uint8_t value = 0xF0;
        static constexpr auto method =
          pgm::Process<ParseInfo(const std::uint8_t *&, std::size_t &)>{}
          << StripStatus
          << CheckLength<1>
          << SwitcherFactory::Parse<ParseInfo(const std::uint8_t *, std::size_t), CaseList>(
            GetFirstByte,
            pgm::Process<ParseInfo(const std::uint8_t *&, std::size_t &)>{} << specificMatchFunction << interpretSpecificSysEx);

        static constexpr auto insight = [](auto...) { return MidiSize::Syx(); };
      };
      struct MTC : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xF1;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
        static constexpr auto insight = [](auto...) { return MidiSize::Match<2>(); };
      };
      struct Songpos : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xF2;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
        static constexpr auto insight = [](auto...) { return MidiSize::Match<3>(); };
      };
      struct SongSel : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xF3;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
        static constexpr auto insight = [](auto...) { return MidiSize::Match<2>(); };
      };
      struct TuneRequest : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xF6;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
        static constexpr auto insight = [](auto...) { return MidiSize::Match<1>(); };
      };
      // System Real Time
      struct TimingClock : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xF8;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
        static constexpr auto insight = [](auto...) { return MidiSize::Match<1>(); };
      };
      struct Start : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xFA;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
        static constexpr auto insight = [](auto...) { return MidiSize::Match<1>(); };
      };
      struct Continue : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xFB;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
        static constexpr auto insight = [](auto...) { return MidiSize::Match<1>(); };
      };
      struct Stop : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xFC;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
        static constexpr auto insight = [](auto...) { return MidiSize::Match<1>(); };
      };
      struct ActiveSensing : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xFE;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
        static constexpr auto insight = [](auto...) { return MidiSize::Match<1>(); };
      };
      struct SystemReset : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xFF;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
        static constexpr auto insight = [](auto...) { return MidiSize::Match<1>(); };
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
      static constexpr auto method = pgm::Process<ParseInfo(const std::uint8_t *&, std::size_t &)>{}
        << SwitcherFactory::Parse<ParseInfo(const std::uint8_t *, std::size_t), CaseList>(
          GetFirstByte,
          InvalidParse);

      static constexpr auto insight = SwitcherFactory::Size<MidiSize(std::uint8_t), CaseList>(
        u8Forward,
        InvalidInsight);
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
    static constexpr auto method = SwitcherFactory::Parse<ParseInfo(const std::uint8_t *, std::size_t), CaseList>(
      GetFirstByteMask<0xF0>,
      InvalidParse);

    static constexpr auto insight = SwitcherFactory::Size<MidiSize(std::uint8_t), CaseList>(
      u8Mask<0xF0>,
      InvalidInsight);
  };

  struct M2 : NotInstantiable
  {
  public:
    struct Utility : NotInstantiable
    {
      struct NOOP : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x00;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct JRClock : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x10;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct JRTimestamp : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x20;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };

    private:
      using CaseList = std::tuple<
        NOOP,
        JRClock,
        JRTimestamp>;

    public:
      static constexpr std::uint8_t value = 0x00;
      static constexpr auto method = SwitcherFactory::Parse<ParseInfo(const std::uint8_t *, std::size_t), CaseList>(
        GetByteMask<1, 0xF0>,
        InvalidParse);

      static constexpr auto insight = [](auto...) { return MidiSize::Match<4>(); };
    };
    struct System : NotInstantiable
    {
      struct MTC : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xF1;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct SongPos : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xF2;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct SongSel : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xF3;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct TuneRequest : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xF6;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct TimingClock : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xF8;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct Start : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xFA;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct Continue : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xFB;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct Stop : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xFC;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct ActiveSensing : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xFE;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct Reset : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xFF;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
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
      static constexpr auto method = SwitcherFactory::Parse<ParseInfo(const std::uint8_t *, std::size_t), CaseList>(
        GetByteMask<1, 0xFF>,
        InvalidParse);

      static constexpr auto insight = [](auto...) { return MidiSize::Match<4>(); };
    };
    struct Midi1Channel : NotInstantiable
    {
      struct NoteOff : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x80;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct NoteOn : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x90;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct PolyPressure : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xA0;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct ControlChange : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xB0;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct ProgramChange : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xC0;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct ChannelPressure : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xD0;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct PitchBend : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xE0;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
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
      static constexpr auto method = SwitcherFactory::Parse<ParseInfo(const std::uint8_t *, std::size_t), CaseList>(
        GetByteMask<1, 0xF0>,
        InvalidParse);

      static constexpr auto insight = [](auto...) { return MidiSize::Match<4>(); };
    };
    struct Data64Bits : NotInstantiable
    {
      struct SysEx1Packet : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x00;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct SysExStart : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x10;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct SysExContinue : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x20;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct SysExEnd : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x30;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };

    private:
      using CaseList = std::tuple<
        SysEx1Packet,
        SysExStart,
        SysExContinue,
        SysExEnd>;

    public:
      static constexpr std::uint8_t value = 0x30;
      static constexpr auto method = pgm::Process<ParseInfo(const std::uint8_t *, std::size_t)>{}
        << CheckLength<8>
        << SwitcherFactory::Parse<ParseInfo(const std::uint8_t *, std::size_t), CaseList>(
          GetByteMask<1, 0xF0>,
          InvalidParse);

      static constexpr auto insight = [](auto...) { return MidiSize::Match<8>(); };
    };
    struct Midi2Channel : NotInstantiable
    {
      struct RegistPerNoteCtrl : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x00;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct AssignPerNoteCtrl : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x10;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct RegistCtrl : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x20;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct AssignCtrl : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x30;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct RelativeRegistCtrl : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x40;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct RelativeAssignCtrl : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x60;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct PerNotePitchBend : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x70;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct NoteOff : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x80;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct NoteOn : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x90;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct PolyPressure : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xA0;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct ControlChange : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xB0;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct ProgramChange : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xC0;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct ChannelPressure : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xD0;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct PitchBend : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xE0;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct PerNoteManagement : NotInstantiable
      {
        static constexpr std::uint8_t value = 0xF0;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
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
      static constexpr auto method = pgm::Process<ParseInfo(const std::uint8_t *, std::size_t)>{}
        << CheckLength<8>
        << SwitcherFactory::Parse<ParseInfo(const std::uint8_t *, std::size_t), CaseList>(
          GetByteMask<1, 0xF0>,
          InvalidParse);

      static constexpr auto insight = [](auto...) { return MidiSize::Match<8>(); };
    };
    struct Data128Bits : NotInstantiable
    {
      struct SysEx8In1Packet : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x00;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct SysEx8Start : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x10;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct SysEx8Continue : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x20;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct SysEx8End : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x30;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct MixedDataSetHeader : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x80;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
      };
      struct MixedDataSetPayload : NotInstantiable
      {
        static constexpr std::uint8_t value = 0x90;
        static constexpr ParseInfo method(const std::uint8_t *, std::size_t);
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
      static constexpr auto method = pgm::Process<ParseInfo(const std::uint8_t *, std::size_t)>{}
        << CheckLength<16>
        << SwitcherFactory::Parse<ParseInfo(const std::uint8_t *, std::size_t), CaseList>(
          GetByteMask<1, 0xF0>,
          InvalidParse);

      static constexpr auto insight = [](auto...) { return MidiSize::Match<16>(); };
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
    static constexpr bool isMidi2Enabled();

  private:
    static constexpr auto allowMidi2Parse = [](auto...) { return isMidi2Enabled() ? ParseInfo{} : ParseInfo{ParseInfo::E::UNIMPLEMENTED}; };
    static constexpr auto allowMidi2Insight = [](auto...) { return isMidi2Enabled() ? MidiSize{} : MidiSize::Discard<1>(); };

  public:
    static constexpr std::uint8_t value = 0x00;
    static constexpr auto method = pgm::Process<ParseInfo(const std::uint8_t *&, std::size_t &)>{}
      << allowMidi2Parse
      << CheckLength<4>
      << SwitcherFactory::Parse<ParseInfo(const std::uint8_t *, std::size_t), CaseList>(
        GetFirstByteMask<0xF0>,
        InvalidParse);

    static constexpr auto insight = pgm::Process<MidiSize(uint8_t)>()
      << allowMidi2Insight
      << SwitcherFactory::Size<MidiSize(std::uint8_t), CaseList>(
        u8Mask<0xF0>,
        InvalidInsight);
  };

private:
  using CaseList = std::tuple<
    M1,
    M2>;

public:
  [[maybe_unused]] static constexpr auto Interpret = pgm::Process<ParseInfo(const std::uint8_t *&, std::size_t &)>{}
    << CheckLength<1>
    << SwitcherFactory::Parse<ParseInfo(const std::uint8_t *, std::size_t), CaseList>(
      GetFirstByteMask<0x80>,
      InvalidParse);

  [[maybe_unused]] static constexpr auto Insight = SwitcherFactory::Size<MidiSize(std::uint8_t), CaseList>(
    u8Mask<0x80>,
    InvalidInsight);
};

#endif // MIDI_PARSER_HPP
