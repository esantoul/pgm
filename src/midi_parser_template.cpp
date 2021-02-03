#include "midi_parser.hpp"

// PInfo MidiBytes::M1::NoteOff::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::NoteOn::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::PolyPressure::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::ControlChange::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::ProgramChange::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::ChannelPressure::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::PitchBend::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }

// PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::SampleDumpHeader::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::SampleDataPacket::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::SampleDumpRequest::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::MidiTimeCode::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::SampleDumpExtensions::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::GeneralInformation::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::FileDump::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::MidiTuningStandard::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::GeneralMidi::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::EndOfFile::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::Wait::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::Cancel::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::NAK::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::ACK::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }

// PInfo MidiBytes::M1::SystemMessage::SysEx::UniRT::MidiTimeCode::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::SysEx::UniRT::ShowControls::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::SysEx::UniRT::NotationInfo::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::SysEx::UniRT::DeviceControl::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::SysEx::UniRT::RTMTCCue::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::SysEx::UniRT::MMCCommands::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::SysEx::UniRT::MMCResponse::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::SysEx::UniRT::MidiTuning::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }

// PInfo MidiBytes::M1::SystemMessage::MTC::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::Songpos::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::SongSel::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::TuneRequest::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::TimingClock::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::Start::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::Continue::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::Stop::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::ActiveSensing::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::SystemReset::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }




// PInfo MidiBytes::M2::checkMidi2Status(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }

// PInfo MidiBytes::M2::Utility::NOOP::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Utility::JRClock::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Utility::JRTimestamp::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }

// PInfo MidiBytes::M2::System::MTC::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::System::SongPos::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::System::SongSel::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::System::TuneRequest::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::System::TimingClock::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::System::Start::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::System::Continue::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::System::Stop::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::System::ActiveSensing::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::System::Reset::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }

// PInfo MidiBytes::M2::Midi1Channel::NoteOff::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Midi1Channel::NoteOn::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Midi1Channel::PolyPressure::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Midi1Channel::ControlChange::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Midi1Channel::ProgramChange::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Midi1Channel::ChannelPressure::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Midi1Channel::PitchBend::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }

// PInfo MidiBytes::M2::Data64Bits::SysEx1Packet::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Data64Bits::SysExStart::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Data64Bits::SysExContinue::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Data64Bits::SysExEnd::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }

// PInfo MidiBytes::M2::Midi2Channel::RegistPerNoteCtrl::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Midi2Channel::AssignPerNoteCtrl::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Midi2Channel::RegistCtrl::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Midi2Channel::AssignCtrl::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Midi2Channel::RelativeRegistCtrl::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Midi2Channel::RelativeAssignCtrl::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Midi2Channel::PerNotePitchBend::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Midi2Channel::NoteOff::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Midi2Channel::NoteOn::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Midi2Channel::PolyPressure::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Midi2Channel::ControlChange::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Midi2Channel::ProgramChange::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Midi2Channel::ChannelPressure::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Midi2Channel::PitchBend::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Midi2Channel::PerNoteManagement::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }

// PInfo MidiBytes::M2::Data128Bits::SysEx8In1Packet::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Data128Bits::SysEx8Start::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Data128Bits::SysEx8Continue::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Data128Bits::SysEx8End::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Data128Bits::MixedDataSetHeader::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M2::Data128Bits::MixedDataSetPayload::method(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }


// PInfo MidiBytes::M1::SystemMessage::SysEx::specificMatchFunction(const std::uint8_t *&, std::size_t &) { return {PInfo::E::UNIMPLEMENTED}; }
// PInfo MidiBytes::M1::SystemMessage::SysEx::interpretSpecificSysEx(const std::uint8_t *, std::size_t) { return {PInfo::E::UNIMPLEMENTED}; }


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
  //*
  // uint8_t bytes[] = {0xF0, 0x7E, 0x69, 0x07, 0x03, 0x42, 'B', 'I', 'N', ' ', 'e', 'r', 'a', 'e', '_', 't', 'o', 'u', 'c', 'h', '_', 's', 'o', 'f', 't', 0xF7};
  // uint8_t bytes[] = {0xF0, 0x7E, 0x7F, 0x06, 0x01, 0xF7};
  // uint8_t bytes[] = {0x94, 0x40, 0x7F};
  uint8_t bytes[] = {0xF0, 0x7F, 0x69, 0x04, 0x01, 0x00, 0x00, 0xF7};

  PInfo ret = MidiBytes::Interpret(bytes, sizeof(bytes));

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

  PInfo ret = MidiBytes::Interpret(parsed.value().data(), parsed.value().size());

  //*/

  switch (ret.status())
  {
  case PInfo::E::UNDEFINED:
    printf("Undefined return status\n");
    break;
  case PInfo::E::SUCCESS:
    printf("Message parsed successfully!\n");
    break;
  case PInfo::E::UNIMPLEMENTED:
    printf("Message type not implemented\n");
    break;
  case PInfo::E::ERROR_MSG_TOO_SHORT:
    printf("Error: Message is too short\n");
    break;
  case PInfo::E::ERROR_UNADEQUATE_LENGTH:
    printf("Error: Unadequate length\n");
    break;
  case PInfo::E::ERROR_UNKNOWN:
    printf("Error: UNKNOWN\n");
    break;
  case PInfo::E::ERROR_INVALID_CASE:
    printf("Error: Invalid Case\n");
    break;
  }

  return 0;
}
