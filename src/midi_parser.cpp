#include "midi_parser.hpp"

#include <cstdio>
#include <utility>
#include <algorithm>

class glob_msg
{
public:
  static constexpr std::size_t maxlen = 64;

  static void print() { printf("%s\n", mData); }

  template <std::size_t N>
  static constexpr void set(const char(&msg)[N])
  {
    for (std::size_t i = 0; i < std::min(N, maxlen - 1); ++i)
      mData[i] = msg[i];
  }

private:
  static char mData[maxlen];
};
char glob_msg::mData[]{};

const std::uint8_t *glob_data = nullptr;
std::size_t glob_len = 0;

void prt_glob_data()
{
  while (glob_len--)
    printf("%02X ", *glob_data++);
  glob_data = nullptr;
  glob_len = 0;
  printf("\n");
}

constexpr ParseInfo MidiBytes::M1::NoteOff::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 NoteOff");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::NoteOn::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 NoteOn");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::PolyPressure::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 PolyPressure");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::ControlChange::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 ControlChange");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::ProgramChange::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 ProgramChange");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::ChannelPressure::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 ChannelPressure");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::PitchBend::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 PitchBend");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}


constexpr ParseInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::SampleDumpHeader::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 SampleDumpHeader");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::SampleDataPacket::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 SampleDataPacket");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::SampleDumpRequest::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 SampleDumpRequest");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::MidiTimeCode::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 MidiTimeCode");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::SampleDumpExtensions::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 SampleDumpExtensions");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::GeneralInformation::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 GeneralInformation");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::FileDump::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 FileDump");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::MidiTuningStandard::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 MidiTuningStandard");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::GeneralMidi::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 GeneralMidi");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::EndOfFile::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 EndOfFile");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::Wait::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 Wait");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::Cancel::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 Cancel");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::NAK::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 NAK");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::ACK::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 ACK");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}


constexpr ParseInfo MidiBytes::M1::SystemMessage::SysEx::UniRT::MidiTimeCode::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 MidiTimeCode");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::SysEx::UniRT::ShowControls::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 ShowControls");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::SysEx::UniRT::NotationInfo::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 NotationInfo");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::SysEx::UniRT::DeviceControl::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 DeviceControl");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::SysEx::UniRT::RTMTCCue::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 RTMTCCue");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::SysEx::UniRT::MMCCommands::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 MMCCommands");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::SysEx::UniRT::MMCResponse::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 MMCResponse");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::SysEx::UniRT::MidiTuning::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 MidiTuning");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}


constexpr ParseInfo MidiBytes::M1::SystemMessage::MTC::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 MTC");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::Songpos::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 Songpos");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::SongSel::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 SongSel");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::TuneRequest::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 TuneRequest");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::TimingClock::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 TimingClock");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::Start::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 Start");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::Continue::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 Continue");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::Stop::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 Stop");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::ActiveSensing::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 ActiveSensing");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M1::SystemMessage::SystemReset::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M1 SystemReset");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}





constexpr bool MidiBytes::M2::isMidi2Enabled() { return true; }

constexpr ParseInfo MidiBytes::M2::Utility::NOOP::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 NOOP");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Utility::JRClock::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 JRClock");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Utility::JRTimestamp::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 JRTimestamp");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}


constexpr ParseInfo MidiBytes::M2::System::MTC::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 MTC");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::System::SongPos::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 SongPos");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::System::SongSel::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 SongSel");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::System::TuneRequest::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 TuneRequest");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::System::TimingClock::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 TimingClock");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::System::Start::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 Start");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::System::Continue::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 Continue");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::System::Stop::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 Stop");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::System::ActiveSensing::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 ActiveSensing");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::System::Reset::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 Reset");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}


constexpr ParseInfo MidiBytes::M2::Midi1Channel::NoteOff::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 NoteOff");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Midi1Channel::NoteOn::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 NoteOn");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Midi1Channel::PolyPressure::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 PolyPressure");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Midi1Channel::ControlChange::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 ControlChange");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Midi1Channel::ProgramChange::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 ProgramChange");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Midi1Channel::ChannelPressure::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 ChannelPressure");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Midi1Channel::PitchBend::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 PitchBend");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}


constexpr ParseInfo MidiBytes::M2::Data64Bits::SysEx1Packet::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 SysEx1Packet");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Data64Bits::SysExStart::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 SysExStart");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Data64Bits::SysExContinue::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 SysExContinue");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Data64Bits::SysExEnd::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 SysExEnd");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}


constexpr ParseInfo MidiBytes::M2::Midi2Channel::RegistPerNoteCtrl::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 RegistPerNoteCtrl");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Midi2Channel::AssignPerNoteCtrl::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 AssignPerNoteCtrl");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Midi2Channel::RegistCtrl::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 RegistCtrl");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Midi2Channel::AssignCtrl::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 AssignCtrl");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Midi2Channel::RelativeRegistCtrl::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 RelativeRegistCtrl");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Midi2Channel::RelativeAssignCtrl::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 RelativeAssignCtrl");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Midi2Channel::PerNotePitchBend::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 PerNotePitchBend");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Midi2Channel::NoteOff::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 NoteOff");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Midi2Channel::NoteOn::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 NoteOn");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Midi2Channel::PolyPressure::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 PolyPressure");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Midi2Channel::ControlChange::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 ControlChange");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Midi2Channel::ProgramChange::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 ProgramChange");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Midi2Channel::ChannelPressure::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 ChannelPressure");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Midi2Channel::PitchBend::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 PitchBend");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Midi2Channel::PerNoteManagement::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 PerNoteManagement");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}


constexpr ParseInfo MidiBytes::M2::Data128Bits::SysEx8In1Packet::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 SysEx8In1Packet");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Data128Bits::SysEx8Start::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 SysEx8Start");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Data128Bits::SysEx8Continue::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 SysEx8Continue");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Data128Bits::SysEx8End::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 SysEx8End");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Data128Bits::MixedDataSetHeader::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 MixedDataSetHeader");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}

constexpr ParseInfo MidiBytes::M2::Data128Bits::MixedDataSetPayload::method(const std::uint8_t *bytes, std::size_t length)
{
  glob_msg::set("M2 MixedDataSetPayload");
  glob_data = bytes;
  glob_len = length;
  return {ParseInfo::E::SUCCESS};
}



constexpr ParseInfo MidiBytes::M1::SystemMessage::SysEx::specificMatchFunction(const std::uint8_t *&, std::size_t &) { return {ParseInfo::E::UNIMPLEMENTED}; }
constexpr ParseInfo MidiBytes::M1::SystemMessage::SysEx::interpretSpecificSysEx(const std::uint8_t *, std::size_t) { return {ParseInfo::E::UNIMPLEMENTED}; }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <cstring>
#include "static_vector.h"
#include <optional>

constexpr std::optional<uint8_t> char2hex(signed char c)
{
  if (c >= 'a' && c <= 'f')
    c += 'A' - 'a';
  if ((c >= '0' && c <= '9'))
    return c - '0';
  else if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;
  else
    return {};
}

constexpr std::optional<uint8_t> parse2char(const char str[])
{
  auto mshB = char2hex(str[0]);
  auto lshB = char2hex(str[1]);
  if (mshB && lshB)
    return mshB.value() << 4 | lshB.value();
  else
    return {};
}

constexpr std::optional<utils::StaticVector<uint8_t, 128>> parse_args(const int argc, const char *argv[])
{
  utils::StaticVector<uint8_t, 128> bytes;
  for (int i = 0; i < argc; ++i)
  {
    auto len = strlen(argv[i]);
    if ((len != 2) && (len != 4))
      return {};
    std::optional<uint8_t> num;
    if (len == 2)
      num = parse2char(argv[i]);
    else
      num = parse2char(argv[i] + 2);
    if (num)
      bytes.push_back(num.value());
    else
      return {};
  }
  if (bytes.size())
    return bytes;
  else
    return {};
}

int main([[maybe_unused]] int argc, [[maybe_unused]] const char *argv[])
{
  // OLD VERSION, static message
  /*
  // uint8_t bytes[] = {0xF0, 0x7E, 0x69, 0x07, 0x03, 0x42, 'B', 'I', 'N', ' ', 'e', 'r', 'a', 'e', '_', 't', 'o', 'u', 'c', 'h', '_', 's', 'o', 'f', 't', 0xF7};
  // uint8_t bytes[] = {0xF0, 0x7E, 0x7F, 0x06, 0x01, 0xF7};
  // uint8_t bytes[] = {0x94, 0x40, 0x7F};
  uint8_t bytes[] = {0xF0, 0x7F, 0x69, 0x04, 0x01, 0x00, 0x00, 0xF7};

  ParseInfo ret = MidiBytes::Interpret(bytes, sizeof(bytes));

  /*/ // New version, message from call arguments such as "./program_name 94 3F 7F"

  auto parsed = parse_args(argc - 1, argv + 1);
  if (!parsed)
  {
    printf("Invalid input\n");
    return 0;
  }

  printf("Input data: \n");
  for (const auto &el : parsed.value())
    printf("%02X ", el);
  printf("\n");


  MidiSize sz = MidiBytes::Insight(parsed.value()[0]);

  switch (sz.status())
  {
  case MidiSize::Status::Discard:
    printf("Next %d bytes should be discarted\n", sz.value());
    break;
  case MidiSize::Status::Set:
    printf("Next message size is %d bytes\n", sz.value());
    break;
  case MidiSize::Status::SysEx:
    printf("Next message is a SysEx\n");
    break;
  case MidiSize::Status::Unknown:
    printf("Unexpected byte!\n");
    break;
  case MidiSize::Status::Error:
    printf("Unexpected ERROR!\n");
    break;
  }


  ParseInfo ret = MidiBytes::Interpret(parsed.value().data(), parsed.value().size());

  glob_msg::print();
  prt_glob_data();

  //*/

  switch (ret.status())
  {
  case ParseInfo::E::UNDEFINED:
    printf("Undefined return status\n");
    break;
  case ParseInfo::E::SUCCESS:
    printf("Message parsed successfully!\n");
    break;
  case ParseInfo::E::UNIMPLEMENTED:
    printf("Message type not implemented\n");
    break;
  case ParseInfo::E::ERROR_MSG_TOO_SHORT:
    printf("Error: Message is too short\n");
    break;
  case ParseInfo::E::ERROR_UNADEQUATE_LENGTH:
    printf("Error: Unadequate length\n");
    break;
  case ParseInfo::E::ERROR_UNKNOWN:
    printf("Error: UNKNOWN\n");
    break;
  case ParseInfo::E::ERROR_INVALID_CASE:
    printf("Error: Invalid Case\n");
    break;
  }

  return 0;
}
