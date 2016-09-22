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
#include "../core/Common.h"
#include "../core/Events.h"
#include "../core/JsonIO.h"
#include "../core/Params.h"

using MidiDeviceId = int;
using MidiChannel = int;

const MidiDeviceId NO_MIDI_DEVICE = -1;

enum class MidiMessageType {
  CONTROL_CHANGE,
  NOTE_ON,
  NOTE_OFF,
  OTHER
};

extern EnumTypeInfo<MidiMessageType> MidiMessageTypeInfo;
std::ostream& operator<<(std::ostream& os,
                         const MidiMessageType& messageType);

MidiMessageType statusToMessageType(const MidiStatus& status);

class MidiMappingKey
: public Outputable
, public JsonReadable
, public JsonWritable {
public:
  static MidiMappingKey create(const MidiDeviceId& device,
                               const ofxMidiMessage& message);

  MidiMappingKey()
  : _device(NO_MIDI_DEVICE)
  , _type(MidiMessageType::OTHER)
  , _channel(0)
  , _cc(0) {}

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
  const int& cc() const { return _cc; }

  Json to_json() const override;
  void read_json(const Json& obj) override;

  std::size_t hash() const;

  std::string typeName() const override { return "MidiMappingKey"; }
protected:
  void outputFields(std::ostream& os) const override;
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
: public EventArgs {
public:
  MidiReceivedEventArgs(const MidiDeviceId& dev,
                        const ofxMidiMessage& msg)
  : device(dev)
  , message(msg)
  , key(MidiMappingKey::create(dev, msg)) { }

  const MidiDeviceId device;
  const ofxMidiMessage& message;
  const MidiMappingKey key;

  std::string typeName() const override { return "MidiReceivedEventArgs"; }
protected:
  void outputFields(std::ostream& os) const override;
};

using MidiReceivedEvent = TEvent<MidiReceivedEventArgs>;

using MidiDeviceParams = ParamsWithEnabled;

class MidiParams
: public ParamsWithEnabled {
public:
  MidiParams() {
    add(twister
        .setKey("twister")
        .setName("MF Twister"));
    add(max
        .setKey("max")
        .setName("Max/MSP Relay"));
  }

  MidiDeviceParams twister;
  MidiDeviceParams max;
};

#endif /* MidiCommon_h */
