//
//  MidiCommon.h
//  memory
//
//  Created by tekt on 7/15/16.
//
//

#ifndef MidiCommon_h
#define MidiCommon_h

#include <functional>
#include <iostream>
#include <JsonIO.h>
#include <ofxMidi.h>
#include "Common.h"
#include "Events.h"

using MidiDeviceId = long;
using MidiChannel = int;

enum class MidiMessageType {
  CONTROL_CHANGE,
  NOTE_ON,
  NOTE_OFF,
  OTHER
};

extern EnumTypeInfo<MidiMessageType> MidiMessageTypeType;
std::ostream& operator<<(std::ostream& os,
                         const MidiMessageType& messageType);

MidiMessageType statusToMessageType(const MidiStatus& status);

class MidiMappingKey
: public Outputable {
public:
  static MidiMappingKey create(const MidiDeviceId& device,
                               const ofxMidiMessage& message);

  MidiMappingKey(MidiDeviceId device,
                 MidiMessageType type,
                 MidiChannel channel,
                 int cc)
  : _device(device)
  , _type(type)
  , _channel(channel)
  , _cc(cc) {}

  const MidiDeviceId& device() const { return _device; }
  const MidiMessageType& type() const { return _type; }
  const MidiChannel& channel() const { return _channel; }

  void output(std::ostream& os) const;

  Json to_json() const;
  void read_json(const Json& obj);

  std::size_t hash() const;
private:
  MidiDeviceId _device;
  MidiMessageType _type;
  MidiChannel _channel;
  int _cc;

  friend bool operator==(const MidiMappingKey& lha,
                         const MidiMappingKey& rha);
};

bool operator==(const MidiMappingKey& lha, const MidiMappingKey& rha);

namespace std {
  template<>
  struct hash<MidiMappingKey> {
    typedef MidiMappingKey argument_type;
    typedef std::size_t result_type;

    result_type operator()(const argument_type& mapping) const {
      return mapping.hash();
    }
  };
}

class MidiReceivedEventArgs
: public Outputable
, public EventArgs {
public:
  MidiReceivedEventArgs(const MidiDeviceId& dev,
                        const ofxMidiMessage& msg)
  : device(dev)
  , message(msg)
  , key(MidiMappingKey::create(dev, msg)) { }

  void output(std::ostream& os) const override;

  const MidiDeviceId device;
  const ofxMidiMessage& message;
  const MidiMappingKey key;
};

using MidiReceivedEvent = TEvent<MidiReceivedEventArgs>;

#endif /* MidiCommon_h */
