//
//  MidiRouter.h
//  memory
//
//  Created by tekt on 7/15/16.
//
//

#ifndef MidiRouter_h
#define MidiRouter_h

#include <initializer_list>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include "../app/AppParameters.h"
#include "../control/MidiDevice.h"
#include "../control/MidiEvents.h"
#include "../control/MidiMapping.h"
#include "../control/Params.h"
#include "../core/Common.h"
#include "../core/JsonIO.h"

class AbstractMidiBinding;

class MidiRouter
: public NonCopyable {
public:
  using DeviceMap = std::unordered_map<MidiDeviceId, std::shared_ptr<MidiDevice>>;
  using BindingMap = std::unordered_map<MidiMappingKey, std::shared_ptr<AbstractMidiBinding>>;
  using DeviceIdMap = std::unordered_map<std::string, MidiDeviceId>;

  MidiRouter(MemoryAppParameters& appParams)
  : _appParams(appParams) { }

  void setup(std::initializer_list<std::shared_ptr<MidiDevice>> devices);

  std::shared_ptr<MidiDevice> getDevice(MidiDeviceId id);
  MidiDeviceId getDeviceId(const std::string& name) {
    return _deviceNameToId[name];
  }
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
  std::shared_ptr<MidiEventRouter> _eventRouter;
};

#endif /* MidiRouter_h */
