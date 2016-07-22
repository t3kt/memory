//
//  ControlCommon.cpp
//  memory
//
//  Created by tekt on 7/15/16.
//
//

#include <boost/functional/hash.hpp>
#include "ControlCommon.h"

ControlDeviceId nextDeviceId() {
  static ControlDeviceId lastId = 0;
  return ++lastId;
}

EnumTypeInfo<ControlDeviceType> ControlDeviceTypeType {
  {"midi", ControlDeviceType::MIDI},
  {"osc", ControlDeviceType::OSC},
};

std::ostream& operator<<(std::ostream& os,
                         const ControlDeviceType& value) {
  return os << ControlDeviceTypeType.toString(value);
}

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

ControlMappingKey
ControlMappingKey::createMidi(const ControlDeviceId& device,
                              const ofxMidiMessage& message) {
  ControlMappingKey key;
  key._deviceType = ControlDeviceType::MIDI;
  key._device = device;
  key._midiType = statusToMessageType(message.status);
  key._channel = message.channel;
  key._cc = message.control;
  return key;
}

void ControlMappingKey::outputFields(std::ostream& os) const {
  os << "devType: " << _deviceType;
  os << "dev: " << _device;
  switch (_deviceType) {
    case ControlDeviceType::MIDI:
      os << ", midiType: " << _midiType;
      os << ", chan: " << _channel;
      os << ", cc: " << _cc;
      break;
    case ControlDeviceType::OSC:
      break;
  }
  return os;
}

bool operator==(const ControlMappingKey& lha,
                const ControlMappingKey& rha) {
  // omit device type since device id is enough for identity
  return lha._device == rha._device
      && lha._midiType == rha._midiType
      && lha._channel == rha._channel
      && lha._cc == rha._cc;
}

std::size_t ControlMappingKey::hash() const {
  std::size_t seed = 0;
  // omit device type since device id is enough for identity
  boost::hash_combine(seed, _device);
  boost::hash_combine(seed, _midiType);
  boost::hash_combine(seed, _channel);
  boost::hash_combine(seed, _cc);
  return seed;
}

namespace JsonUtil {
  template<>
  Json toJson(const MidiMessageType& value) {
    return MidiMessageTypeType.toString(value);
  }

  template<>
  MidiMessageType fromJson<MidiMessageType>(const Json& value) {
    assertHasType(value, Json::STRING);
    return MidiMessageTypeType.parseString(value.string_value());
  }

  template<>
  Json toJson(const ControlDeviceType& value) {
    return ControlDeviceTypeType.toString(value);
  }

  template<>
  ControlDeviceType fromJson<ControlDeviceType>(const Json& value) {
    assertHasType(value, Json::STRING);
    return ControlDeviceTypeType.parseString(value.string_value());
  }
}

Json ControlMappingKey::to_json() const {
  switch (_deviceType) {
    case ControlDeviceType::MIDI:
      return Json::object {
        {"deviceType", JsonUtil::toJson(_deviceType)},
        {"device", JsonUtil::toJson(_device)},
        {"channel", JsonUtil::toJson(_channel)},
        {"midiType", JsonUtil::toJson(_midiType)},
        {"cc", JsonUtil::toJson(_cc)},
      };
    case ControlDeviceType::OSC:
      return Json::object {
        {"deviceType", JsonUtil::toJson(_deviceType)},
        {"device", JsonUtil::toJson(_device)},
      };
    default:
      throw std::invalid_argument("Unknown device type");
  }
}

void ControlMappingKey::read_json(const Json &obj) {
  JsonUtil::assertHasType(obj, Json::OBJECT);
  _deviceType = JsonUtil::fromJson<ControlDeviceType>(obj["deviceType"]);
  _device = JsonUtil::fromJson<ControlDeviceId>(obj["device"]);
  switch (_deviceType) {
    case ControlDeviceType::MIDI:
      _channel = JsonUtil::fromJson<MidiChannel>(obj["channel"]);
      _midiType = JsonUtil::fromJson<MidiMessageType>(obj["midiType"]);
      _cc = JsonUtil::fromJson<ControlDeviceId>(obj["cc"]);
      break;
    case ControlDeviceType::OSC:
      break;
  }
}

void MidiReceivedEventArgs::outputFields(std::ostream &os) const {
  os << "dev:" << device
  << ", msg: " << const_cast<ofxMidiMessage&>(message).toString();
}
