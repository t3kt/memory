//
//  MidiEvents.cpp
//  memory
//
//  Created by tekt on 7/27/16.
//
//

#include "../app/AppSystem.h"
#include "../control/MidiDevice.h"
#include "../control/MidiEvents.h"
#include "../control/MidiRouter.h"

class MidiEventBinding {
public:
  MidiEventBinding(const MidiEventMapping& mapping,
                   std::shared_ptr<MidiDevice> device)
  : _mapping(mapping)
  , _device(device) { }

  virtual ~MidiEventBinding() { }

  virtual void attach(AbstractEvent& event) {
    event.addVoidListener([&]() {
      _device->sendMessage(_mapping.key().type(),
                           _mapping.key().channel(),
                           _mapping.key().cc(),
                           _mapping.value());
      if (_mapping.key().type() == MidiMessageType::NOTE_ON
          && _mapping.autoOff()) {
        _device->sendMessage(MidiMessageType::NOTE_OFF,
                             _mapping.key().channel(),
                             _mapping.key().cc(),
                             _mapping.value());
      }
    }, this);
  }

  virtual void detach(AbstractEvent& event) {
    event.removeListeners(this);
  }

  SimulationEventType eventType() const { return _mapping.eventType(); }

private:
  const MidiEventMapping& _mapping;
  std::shared_ptr<MidiDevice> _device;
};

void MidiEventMapping::outputFields(std::ostream &os) const {
  os << "event: " << _eventType;
  os << ", key: " << _key;
  os << ", val: " << _value;
  if (_key.type() == MidiMessageType::NOTE_ON) {
    os << ", autoOff: " << _autoOff;
  }
}

Json MidiEventMapping::to_json() const {
  Json::object obj {
    {"event", JsonUtil::toJson(_eventType)},
    {"key", _key},
    {"value", JsonUtil::toJson(_value)},
  };
  if (_key.type() == MidiMessageType::NOTE_ON) {
    obj["autoOff"] = _autoOff;
  }
  return obj;
}

void MidiEventMapping::read_json(const Json &obj) {
  JsonUtil::assertHasType(obj, Json::OBJECT);
  _eventType = JsonUtil::fromJson<SimulationEventType>(obj["event"]);
  _key.read_json(obj["key"]);
  _value = JsonUtil::fromJson<int>(obj["value"]);
  _autoOff = JsonUtil::fromJsonField(obj, "autoOff", true);
}

void MidiEventRouter::setup() {
  loadMappings();
}

void MidiEventRouter::attach(SimulationEvents &events) {
  for (auto& binding : _bindings) {
    auto event = events.getEvent(binding->eventType());
    if (event) {
      binding->attach(*event);
    }
  }
}

void MidiEventRouter::detach(SimulationEvents &events) {
  for (auto& binding : _bindings) {
    auto event = events.getEvent(binding->eventType());
    if (event) {
      binding->detach(*event);
    }
  }
}

static MidiEventMapping mapNoteOut(SimulationEventType eventType,
                                   MidiDeviceId deviceId,
                                   MidiChannel channel,
                                   int key) {
  return MidiEventMapping(eventType,
                          MidiMappingKey(deviceId,
                                         MidiMessageType::NOTE_ON,
                                         channel,
                                         key),
                          127);
}

void MidiEventRouter::loadMappings() {
  _mappings.readFromFile("eventMappings.json");
  if (_mappings.empty()) {
    auto devid = _router.getDeviceId("max");
    _mappings.add(mapNoteOut(SimulationEventType::OBSERVER_SPAWNED,
                             devid,
                             1,
                             10));
    _mappings.add(mapNoteOut(SimulationEventType::OBSERVER_DIED,
                             devid,
                             1,
                             20));
    _mappings.add(mapNoteOut(SimulationEventType::OCCURRENCE_SPAWNED,
                             devid,
                             2,
                             10));
//    _mappings.add(mapNoteOut(SimulationEventType::OCCURRENCE_SPAWN_FAILED,
//                             devid,
//                             2,
//                             15));
    _mappings.add(mapNoteOut(SimulationEventType::OCCURRENCE_DIED,
                             devid,
                             2,
                             20));
  }
  initBindings();
}

void MidiEventRouter::initBindings() {
  for (const auto& mapping : _mappings) {
    addBinding(mapping);
  }
}

void MidiEventRouter::addBinding(const MidiEventMapping &mapping) {
  auto device = _router.getDevice(mapping.key().device());
  if (!device) {
    AppSystem::get().log().control().logWarning("Unable to find device for mapping " + ofToString(mapping));
    return;
  }
  auto binding = std::make_shared<MidiEventBinding>(mapping,
                                                    device);
  _bindings.push_back(binding);
}
