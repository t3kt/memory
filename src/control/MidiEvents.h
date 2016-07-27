//
//  MidiEvents.h
//  memory
//
//  Created by tekt on 7/27/16.
//
//

#ifndef MidiEvents_h
#define MidiEvents_h

#include "Common.h"
#include "Events.h"
#include "JsonIO.h"
#include "MidiCommon.h"
#include "Params.h"
#include "SimulationEvents.h"

class MidiEventMapping
: public Outputable
, public JsonReadable
, public JsonWritable {
public:
  MidiEventMapping()
  : _eventType(SimulationEventType::ANIMATION_SPAWNED)
  , _messageType(MidiMessageType::OTHER)
  , _channel(0)
  , _cc(0)
  , _value(0) { }
  MidiEventMapping(SimulationEventType eventType,
                   MidiMessageType messageType,
                   MidiChannel channel,
                   int cc,
                   int value)
  : _eventType(eventType)
  , _messageType(messageType)
  , _channel(channel)
  , _cc(cc)
  , _value(value) { }

  SimulationEventType eventType() const { return _eventType; }
  MidiMessageType messageType() const { return _messageType; }
  MidiChannel channel() const { return _channel; }
  int cc() const { return _cc; }

  Json to_json() const override;
  void read_json(const Json& obj) override;

protected:
  std::string typeName() const override { return "MidiEventMapping"; }
  void outputFields(std::ostream& os) const override;

private:
  SimulationEventType _eventType;
  MidiMessageType _messageType;
  MidiChannel _channel;
  int _cc;
  int _value;
};

#endif /* MidiEvents_h */
