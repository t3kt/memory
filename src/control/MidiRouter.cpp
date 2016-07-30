//
//  MidiRouter.cpp
//  memory
//
//  Created by tekt on 7/15/16.
//
//

#include <stdexcept>
#include <typeinfo>
#include "AppSystem.h"
#include "Common.h"
#include "MidiRouter.h"
#include "SimulationApp.h"

class AbstractMidiBinding {
public:
  virtual ~AbstractMidiBinding() { }
  virtual void receiveValue(int value) = 0;
};

template<typename T>
class MidiBinding
: public AbstractMidiBinding {
public:
  MidiBinding(const MidiMappingKey& key,
              TParam<T>& param,
              std::shared_ptr<MidiDevice> device)
  : _key(key)
  , _param(param)
  , _device(device) {
    _param.changed.addListener([&](T&) {
      sendValue();
    }, this);
  }

  ~MidiBinding() override {
    _param.changed.removeListeners(this);
  }

  void receiveValue(int value) override {
    _param.setNormalizedValue(value / 128.0f);
  }
protected:
  void sendValue() {
    int mesgVal = static_cast<int>(_param.getNormalizedValue() * 128.0f);
    _device->sendMessage(_key.type(),
                         _key.channel(),
                         _key.cc(),
                         mesgVal);
  }

  const MidiMappingKey& _key;
  std::shared_ptr<MidiDevice> _device;
  TParam<T>& _param;
};

template<typename T>
static std::shared_ptr<AbstractMidiBinding>
createTypedBinding(const MidiMappingKey& key,
                   TParamBase& param,
                   std::shared_ptr<MidiDevice> device) {
  TParam<T>& typedParam = dynamic_cast<TParam<T>&>(param);
  return std::shared_ptr<AbstractMidiBinding>(new MidiBinding<T>(key,
                                                                 typedParam,
                                                                 device));
}

static std::shared_ptr<AbstractMidiBinding>
createBinding(const MidiMappingKey& key,
              TParamBase& param,
              std::shared_ptr<MidiDevice> device) {
  if (param.isGroup()) {
    throw std::invalid_argument("Cannot bind to group " + param.getKey());
  }
  const auto& type = param.getTypeInfo();
  if (type == typeid(float)) {
    return createTypedBinding<float>(key, param, device);
  } else if (type == typeid(int)) {
    return createTypedBinding<int>(key, param, device);
  } else if (type == typeid(bool)) {
    return createTypedBinding<bool>(key, param, device);
  } else {
    throw std::invalid_argument("Cannot bind to param " + param.getKey() + " of type " + type.name());
  }
}

void MidiRouter::setup(std::initializer_list<std::shared_ptr<MidiDevice>> devices) {
  for (auto& device : devices) {
    addDevice(device);
  }
  loadMappings();
  _eventRouter = std::make_shared<MidiEventRouter>(*this);
  _eventRouter->setup();
  _eventRouter->attach(AppSystem::get().simulation()->getEvents());
}

void MidiRouter::addDevice(std::shared_ptr<MidiDevice> device) {
  _deviceNameToId[device->name()] = device->id();
  _devices[device->id()] = device;
  device->messageReceived += [&](MidiReceivedEventArgs& event) {
    onInputMessage(event);
  };
}

void MidiRouter::loadMappings() {
  _mappings.readFromFile("mappings.json");
  initBindings();
}

void MidiRouter::initBindings() {
  _bindings.clear();
  for (const auto& mapping : _mappings) {
    addBinding(mapping);
  }
}

void MidiRouter::addBinding(const MidiMapping& mapping) {
  const auto& path = mapping.path();
  if (path.empty()) {
    return;
  }
  TParamBase* param = _appParams.lookupPath(path);
  if (!param) {
    AppSystem::get().log().control().logWarning("Unable to find parameter for binding path '" + path + "'");
    return;
  }
  auto device = getDevice(mapping.key().device());
  if (!device) {
    AppSystem::get().log().control().logWarning("Unable to find device for mapping " + ofToString(mapping));
    return;
  }
  std::shared_ptr<AbstractMidiBinding> binding;
  try {
    binding = createBinding(mapping.key(), *param, device);
  } catch (std::invalid_argument ex) {
    AppSystem::get().log().control().logWarning(std::string("Unable to create binding: ") + ex.what());
    return;
  }
  const auto& key = mapping.key();
  _bindings[key] = binding;
}

std::shared_ptr<MidiDevice> MidiRouter::getDevice(MidiDeviceId id) {
  auto iter = _devices.find(id);
  if (iter == _devices.end()) {
    return std::shared_ptr<MidiDevice>();
  } else {
    return iter->second;
  }
}

void MidiRouter::onInputMessage(MidiReceivedEventArgs &event) {
  if (!_appParams.core.midi.enabled) {
    return;
  }
  auto iter = _bindings.find(event.key);
  if (iter == _bindings.end()) {
    return;
  }
  iter->second->receiveValue(event.message.value);
}
