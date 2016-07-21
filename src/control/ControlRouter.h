//
//  ControlRouter.hControlRouter
//  memory
//
//  Created by tekt on 7/15/16.
//
//

#ifndef ControlRouter_h
#define ControlRouter_h

#include <initializer_list>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include "AppParameters.h"
#include "Events.h"
#include "JsonIO.h"
#include "MidiDevice.h"
#include "MidiMapping.h"
#include "Params.h"

class AbstractMidiBinding;

class ControlRouter {
public:
  using DeviceMap = std::unordered_map<ControlDeviceId, std::shared_ptr<MidiDevice>>;
  using BindingMap = std::unordered_map<ControlMappingKey, std::shared_ptr<AbstractMidiBinding>>;
  using DeviceIdMap = std::unordered_map<std::string, ControlDeviceId>;

  ControlRouter(MemoryAppParameters& appParams)
  : _appParams(appParams) { }

  void setup(std::initializer_list<std::shared_ptr<MidiDevice>> devices);
private:
  void addDevice(std::shared_ptr<MidiDevice> device);

  void loadMappings();
  void initBindings();
  void addBinding(const MidiMapping& mapping);

  void onInputMessage(MidiReceivedEventArgs& event);

  MemoryAppParameters& _appParams;
  MidiMappingSet _mappings;
  BindingMap _bindings;
  DeviceMap _devices;
  DeviceIdMap _deviceNameToId;
};

#endif /* ControlRouter_h */
