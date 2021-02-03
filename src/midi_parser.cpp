#include "midi_parser.hpp"

////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------//

//                                    EXAMPLES

//--------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////

#include <cstdio>
void prtrem(const char *description, const uint8_t *bytes, std::size_t len)
{
  std::printf("%s\n", description);
  for (std::size_t i = 0; i < len; ++i)
    std::printf("%02X ", bytes[i]);
  std::printf("\n");
}

PInfo MidiBytes::M1::NoteOn::method(const uint8_t *bytes, std::size_t length)
{
  if (!HasLength<3>(bytes, length))
    return {PInfo::E::ERROR_UNADEQUATE_LENGTH};
  prtrem("NoteOn", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::NoteOff::method(const uint8_t *bytes, std::size_t length)
{
  if (!HasLength<3>(bytes, length))
    return {PInfo::E::ERROR_UNADEQUATE_LENGTH};
  prtrem("NoteOff", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::PolyPressure::method(const uint8_t *bytes, std::size_t length)
{
  if (!HasLength<3>(bytes, length))
    return {PInfo::E::ERROR_UNADEQUATE_LENGTH};
  prtrem("PolyPressure", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::ControlChange::method(const uint8_t *bytes, std::size_t length)
{
  if (!HasLength<3>(bytes, length))
    return {PInfo::E::ERROR_UNADEQUATE_LENGTH};
  prtrem("ControlChange", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::ProgramChange::method(const uint8_t *bytes, std::size_t length)
{
  if (!HasLength<2>(bytes, length))
    return {PInfo::E::ERROR_UNADEQUATE_LENGTH};
  prtrem("ProgramChange", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::ChannelPressure::method(const uint8_t *bytes, std::size_t length)
{
  if (!HasLength<2>(bytes, length))
    return {PInfo::E::ERROR_UNADEQUATE_LENGTH};
  prtrem("ChannelPressure", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::PitchBend::method(const uint8_t *bytes, std::size_t length)
{
  if (!HasLength<3>(bytes, length))
    return {PInfo::E::ERROR_UNADEQUATE_LENGTH};
  prtrem("PitchBend", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::SampleDumpHeader::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("SampleDumpHeader", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::SampleDataPacket::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("SampleDataPacket", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::SampleDumpRequest::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("SampleDumpRequest", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::MidiTimeCode::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("NRT_MidiTimeCode", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::SampleDumpExtensions::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("SampleDumpExtensions", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::GeneralInformation::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("GeneralInformation", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::FileDump::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("FileDump", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::MidiTuningStandard::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("MidiTuningStandard", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::GeneralMidi::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("GeneralMidi", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::EndOfFile::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("EndOfFile", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::Wait::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("Wait", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::Cancel::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("Cancel", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::NAK::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("NAK", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::SysEx::UniNonRT::ACK::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("ACK", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::SysEx::UniRT::MidiTimeCode::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("RT_MidiTimeCode", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::SysEx::UniRT::ShowControls::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("ShowControls", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::SysEx::UniRT::NotationInfo::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("NotationInfo", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::SysEx::UniRT::DeviceControl::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("DeviceControl", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::SysEx::UniRT::RTMTCCue::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("RTMTCCue", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::SysEx::UniRT::MMCCommands::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("MMCCommands", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::SysEx::UniRT::MMCResponse::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("MMCResponse", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::SysEx::UniRT::MidiTuning::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("MidiTuning", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::MTC::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("MTC", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::Songpos::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("Songpos", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::SongSel::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("SongSel", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::TuneRequest::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("Tune", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::TimingClock::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("Clock", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::Start::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("Start", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::Continue::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("Continue", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::Stop::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("Stop", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::ActiveSensing::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("ActiveSensing", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::SystemReset::method(const uint8_t *bytes, std::size_t length)
{
  prtrem("SystemReset", bytes, length);
  return {PInfo::E::SUCCESS};
}


PInfo MidiBytes::M1::SystemMessage::SysEx::specificMatchFunction(const uint8_t *&bytes, std::size_t &length)
{
  prtrem("specificMatchFunction", bytes, length);
  return {PInfo::E::SUCCESS};
}
PInfo MidiBytes::M1::SystemMessage::SysEx::interpretSpecificSysEx(const uint8_t *bytes, std::size_t length)
{
  prtrem("interpretSpecificSysEx", bytes, length);
  return {PInfo::E::SUCCESS};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cstring>
#include <vector>
#include <optional>

std::optional<uint8_t> char2hex(signed char c)
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

std::optional<uint8_t> parse2char(const char str[])
{
  auto mshB = char2hex(str[0]);
  auto lshB = char2hex(str[1]);
  if (mshB && lshB)
    return mshB.value() << 4 | lshB.value();
  else
    return {};
}

std::optional<std::vector<uint8_t>> parse_args(const int argc, const char *argv[])
{
  std::vector<uint8_t> bytes;
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
