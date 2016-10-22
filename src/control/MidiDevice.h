//
//  MidiDevice.h
//  memory
//
//  Created by tekt on 7/15/16.
//
//

#ifndef MidiDevice_h
#define MidiDevice_h

#include <ofxMidi.h>
#include <string>
#include "../control/MidiCommon.h"
#include "../control/Params.h"
#include "../core/Common.h"

class MidiDevice
: public ofxMidiListener
, public NonCopyable {
public:
  using Params = MidiDeviceParams;

  MidiDevice(std::string name,
             std::string inputPortName,
             std::string outputPortName,
             Params& params);

  virtual ~MidiDevice() {
    handleClose(false);
  }

  const MidiDeviceId& id() const { return _id; }
  const std::string& name() const { return _name; }

  void open() {
    _params.enabled.set(true);
  }

  void close() {
    _params.enabled.set(false);
  }

  MidiReceivedEvent messageReceived;

  void sendMessage(MidiMessageType type,
                   MidiChannel channel,
                   int key,
                   int value);

private:
  void newMidiMessage(ofxMidiMessage& message) override;

  void handleOpen();
  void handleClose(bool updateParams);

  const MidiDeviceId _id;
  const std::string _name;
  const std::string _inputPortName;
  const std::string _outputPortName;
  Params& _params;
  ofxMidiIn _midiIn;
  ofxMidiOut _midiOut;
};

#endif /* MidiDevice_h */
