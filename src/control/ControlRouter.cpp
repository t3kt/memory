//
//  ControlRouter.cpp
//  memory
//
//  Created by tekt on 7/15/16.
//
//

#include <stdexcept>
#include <typeinfo>
#include "Common.h"
#include "ControlRouter.h"

class AbstractMidiBinding {
public:
  virtual void receiveValue(int value) = 0;
};

template<typename T>
class MidiBinding
: public AbstractMidiBinding {
public:
  MidiBinding(TParam<T>& param)
  : _param(param) {}

  void receiveValue(int value) override {
    _param.setNormalizedValue(value / 128.0f);
  }
protected:
  TParam<T>& _param;
};

template<typename T>
static std::shared_ptr<AbstractMidiBinding>
createTypedBinding(TParamBase& param) {
  TParam<T>& typedParam = dynamic_cast<TParam<T>&>(param);
  return std::shared_ptr<AbstractMidiBinding>(new MidiBinding<T>(typedParam));
}

static std::shared_ptr<AbstractMidiBinding>
createBinding(TParamBase& param) {
  if (param.isGroup()) {
    throw std::invalid_argument("Cannot bind to group " + param.getKey());
  }
  const auto& type = param.getTypeInfo();
  if (type == typeid(float)) {
    return createTypedBinding<float>(param);
  } else if (type == typeid(int)) {
    return createTypedBinding<int>(param);
  } else if (type == typeid(bool)) {
    return createTypedBinding<bool>(param);
  } else {
    throw std::invalid_argument("Cannot bind to param " + param.getKey() + " of type " + type.name());
  }
}

void ControlRouter::setup(std::initializer_list<std::shared_ptr<MidiDevice>> devices) {
  for (auto& device : devices) {
    addDevice(device);
  }
  loadMappings();
}

void ControlRouter::addDevice(std::shared_ptr<MidiDevice> device) {
  _deviceNameToId[device->name()] = device->id();
  _devices[device->id()] = device;
  device->messageReceived += [&](MidiReceivedEventArgs& event) {
    onInputMessage(event);
  };
}

void ControlRouter::loadMappings() {
  _mappings.readFromFile("mappings.json");
  initBindings();
}

void ControlRouter::initBindings() {
  _bindings.clear();
  for (const auto& mapping : _mappings) {
    addBinding(mapping);
  }
}

void ControlRouter::addBinding(const MidiMapping& mapping) {
  const auto& path = mapping.path();
  if (path.empty()) {
    return;
  }
  TParamBase* param = _appParams.lookupPath(path);
  if (!param) {
    ofLogWarning() << "Unable to find parameter for binding path '" << path << "'";
  }
  std::shared_ptr<AbstractMidiBinding> binding;
  try {
    binding = createBinding(*param);
  } catch (std::invalid_argument ex) {
    ofLogWarning() << "Unable to create binding: " << ex.what();
    return;
  }
  const auto& key = mapping.key();
  _bindings[key] = binding;
}

void ControlRouter::onInputMessage(MidiReceivedEventArgs &event) {
  if (!_appParams.core.control.enabled
      || !_appParams.core.control.midi.enabled) {
    return;
  }
  auto iter = _bindings.find(event.key);
  if (iter == _bindings.end()) {
    return;
  }
  iter->second->receiveValue(event.message.value);
}
