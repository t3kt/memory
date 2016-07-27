//
//  MidiEvents.cpp
//  memory
//
//  Created by tekt on 7/27/16.
//
//

#include "MidiEvents.h"

void MidiEventMapping::outputFields(std::ostream &os) const {
  os << "event: " << _eventType;
  os << ", message: " << _messageType;
  os << ", channel: " << _channel;
  os << ", cc: " << _cc;
  os << ", val: " << _value;
}

Json MidiEventMapping::to_json() const {
  return Json::object {
    {"event", JsonUtil::toJson(_eventType)},
    {"message", JsonUtil::toJson(_messageType)},
    {"channel", JsonUtil::toJson(_channel)},
    {"cc", JsonUtil::toJson(_cc)},
    {"value", JsonUtil::toJson(_value)},
  };
}

void MidiEventMapping::read_json(const Json &obj) {
  JsonUtil::assertHasType(obj, Json::OBJECT);
  _eventType = JsonUtil::fromJson<SimulationEventType>(obj["event"]);
  _messageType = JsonUtil::fromJson<MidiMessageType>(obj["message"]);
  _channel = JsonUtil::fromJson<MidiChannel>(obj["channel"]);
  _cc = JsonUtil::fromJson<int>(obj["cc"]);
  _value = JsonUtil::fromJson<int>(obj["value"]);
}
