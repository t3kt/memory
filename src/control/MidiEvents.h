//
//  MidiEvents.h
//  memory
//
//  Created by tekt on 7/27/16.
//
//

#ifndef MidiEvents_h
#define MidiEvents_h

#include <vector>
#include "../core/Common.h"
#include "../core/Events.h"
#include "../core/JsonIO.h"
#include "../control/MappingSet.h"
#include "../control/MidiCommon.h"
#include "../core/Params.h"
#include "../core/SimulationEvents.h"

class MidiRouter;

class MidiEventBinding;

class MidiEventMapping
: public Outputable
, public JsonReadable
, public JsonWritable {
public:
  MidiEventMapping()
  : _eventType(SimulationEventType::ANIMATION_SPAWNED)
  , _value(0) { }

  MidiEventMapping(SimulationEventType eventType,
                   MidiMappingKey key,
                   int value,
                   bool autoOff = true)
  : _eventType(eventType)
  , _key(key)
  , _value(value)
  , _autoOff(true) { }

  SimulationEventType eventType() const { return _eventType; }
  const MidiMappingKey& key() const { return _key; }
  int value() const { return _value; }
  bool autoOff() const { return _autoOff; }

  Json to_json() const override;
  void read_json(const Json& obj) override;

  std::string typeName() const override { return "MidiEventMapping"; }
protected:
  void outputFields(std::ostream& os) const override;

private:
  MidiMappingKey _key;
  SimulationEventType _eventType;
  int _value;
  bool _autoOff;
};

using MidiEventMappingSet = MappingSet<MidiEventMapping>;

class MidiEventRouter {
public:
  using BindingList = std::vector<std::shared_ptr<MidiEventBinding>>;

  MidiEventRouter(MidiRouter& router)
  : _router(router) { }

  void setup();

  void attach(SimulationEvents& events);
  void detach(SimulationEvents& events);
private:
  void loadMappings();
  void initBindings();
  void addBinding(const MidiEventMapping& mapping);

  MidiRouter& _router;
  MidiEventMappingSet _mappings;
  BindingList _bindings;
};

#endif /* MidiEvents_h */
