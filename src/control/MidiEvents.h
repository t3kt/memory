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
#include "Common.h"
#include "Events.h"
#include "JsonIO.h"
#include "MappingSet.h"
#include "MidiCommon.h"
#include "Params.h"
#include "SimulationEvents.h"

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
                   int value)
  : _eventType(eventType)
  , _key(key)
  , _value(value) { }

  SimulationEventType eventType() const { return _eventType; }
  const MidiMappingKey& key() const { return _key; }
  int value() const { return _value; }

  Json to_json() const override;
  void read_json(const Json& obj) override;

protected:
  std::string typeName() const override { return "MidiEventMapping"; }
  void outputFields(std::ostream& os) const override;

private:
  MidiMappingKey _key;
  SimulationEventType _eventType;
  int _value;
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
