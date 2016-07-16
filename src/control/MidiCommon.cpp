//
//  MidiCommon.cpp
//  memory
//
//  Created by tekt on 7/15/16.
//
//

#include <boost/functional/hash.hpp>
#include "MidiCommon.h"

EnumTypeInfo<MidiMessageType> MidiMessageTypeType {
  {"cc", MidiMessageType::CONTROL_CHANGE},
  {"noteOn", MidiMessageType::NOTE_ON},
  {"noteOff", MidiMessageType::NOTE_OFF},
  {"other", MidiMessageType::OTHER},
};

std::ostream& operator<<(std::ostream& os,
                         const MidiMessageType& messageType) {
  return os << MidiMessageTypeType.toString(messageType);
}

MidiMessageType statusToMessageType(const MidiStatus& status) {
  switch (status) {
    case MidiStatus::MIDI_CONTROL_CHANGE:
      return MidiMessageType::CONTROL_CHANGE;
    case MidiStatus::MIDI_NOTE_ON:
      return MidiMessageType::NOTE_ON;
    case MidiStatus::MIDI_NOTE_OFF:
      return MidiMessageType::NOTE_OFF;
    default:
      return MidiMessageType::OTHER;
  }
}

MidiMappingKey MidiMappingKey::create(const MidiDeviceId& device,
                                      const ofxMidiMessage& message) {
  return MidiMappingKey(device,
                        statusToMessageType(message.status),
                        message.channel,
                        message.control);
}

void MidiMappingKey::output(std::ostream& os) const {
  return os << "{dev: " << _device
            << ", type: " << _type
            << ", chan: " << _channel
            << ", cc: " << _cc
            << "}";
}

bool operator==(const MidiMappingKey& lha, const MidiMappingKey& rha) {
  return lha._device == rha._device
      && lha._type == rha._type
      && lha._channel == rha._channel
      && lha._cc == rha._cc;
}

std::size_t MidiMappingKey::hash() const {
  std::size_t seed = 0;
  boost::hash_combine(seed, _device);
  boost::hash_combine(seed, _type);
  boost::hash_combine(seed, _channel);
  boost::hash_combine(seed, _cc);
  return seed;
}
