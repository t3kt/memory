//
//  ControlCommon.h
//  memory
//
//  Created by tekt on 7/15/16.
//
//

#ifndef ControlCommon_h
#define ControlCommon_h

#include <functional>
#include <iostream>
#include <JsonIO.h>
#include <ofxMidi.h>
#include "Common.h"
#include "Events.h"
#include "JsonIO.h"
#include "Params.h"

enum class ControlDeviceType {
  MIDI,
  OSC,
};

extern EnumTypeInfo<ControlDeviceType> ControlDeviceTypeType;
std::ostream& operator<<(std::ostream& os,
                         const ControlDeviceType& value);

using ControlDeviceId = int;
using MidiChannel = int;

const ControlDeviceId NO_CONTROL_DEVICE = -1;

ControlDeviceId nextDeviceId();

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

class ControlMappingKey
: public Outputable
, public JsonReadable
, public JsonWritable {
public:
  ControlMappingKey()
  : _deviceType(ControlDeviceType::MIDI)
  , _device(NO_CONTROL_DEVICE)
  , _midiType(MidiMessageType::OTHER)
  , _channel(0)
  , _cc(0)
  , _oscPath("") {}

  static ControlMappingKey createMidi(const ControlDeviceId& device,
                                      const ofxMidiMessage& message);

  const ControlDeviceType& deviceType() const { return _deviceType; }
  const ControlDeviceId& device() const { return _device; }

  const std::string& oscPath() const { return _oscPath; }

  Json to_json() const override;
  void read_json(const Json& obj) override;

  std::size_t hash() const;
protected:
  std::string typeName() const override { return "ControlMappingKey"; }
  void outputFields(std::ostream& os) const override;
private:
  ControlDeviceType _deviceType;
  ControlDeviceId _device;
  MidiMessageType _midiType;
  MidiChannel _channel;
  int _cc;
  std::string _oscPath;

  friend bool operator==(const ControlMappingKey& lha,
                         const ControlMappingKey& rha);
};

bool operator==(const ControlMappingKey& lha,
                const ControlMappingKey& rha);

namespace std {
  template<>
  struct hash<ControlMappingKey> {
    typedef ControlMappingKey argument_type;
    typedef std::size_t result_type;

    result_type operator()(const argument_type& mapping) const {
      return mapping.hash();
    }
  };
}

class MidiReceivedEventArgs
: public EventArgs {
public:
  MidiReceivedEventArgs(const ControlDeviceId& dev,
                        const ofxMidiMessage& msg)
  : device(dev)
  , message(msg)
  , key(ControlMappingKey::createMidi(dev, msg)) { }

  const ControlDeviceId device;
  const ofxMidiMessage& message;
  const ControlMappingKey key;

protected:
  std::string typeName() const override { return "MidiReceivedEventArgs"; }
  void outputFields(std::ostream& os) const override;
};

using MidiReceivedEvent = TEvent<MidiReceivedEventArgs>;

using MidiDeviceParams = ParamsWithEnabled;

using OscParams = ParamsWithEnabled;

class MidiParams
: public ParamsWithEnabled {
public:
  MidiParams() {
    add(twister
        .setKey("twister")
        .setName("MF Twister"));
  }

  MidiDeviceParams twister;
};

class ControlParams
: public ParamsWithEnabled {
public:
  ControlParams() {
    add(midi
        .setKey("midi")
        .setName("Midi"));
    add(osc
        .setKey("osc")
        .setName("OSC"));
  }

  MidiParams midi;
  OscParams osc;
};

#endif /* ControlCommon_h */
