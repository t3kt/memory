//
//  MidiDevice.cpp
//  memory
//
//  Created by tekt on 7/15/16.
//
//

#include "MidiDevice.h"


static MidiDeviceId nextId() {
  static MidiDeviceId lastId = 0;
  return ++lastId;
}

void MidiReceivedEventArgs::output(std::ostream &os) const {
  os << "MidiReceivedEventArgs{dev:" << device
     << ", msg: " << const_cast<ofxMidiMessage&>(message).toString()
     << "}";
}

MidiDevice::MidiDevice(std::string name,
                       std::string inputPortName,
                       std::string outputPortName,
                       Params& params)
: _id(nextId())
, _name(name)
, _inputPortName(inputPortName)
, _outputPortName(outputPortName)
, _params(params) {

  params.enabled.changed.addListener([this](bool& enabled) {
    if (enabled) {
      handleOpen();
    } else {
      handleClose();
    }
  }, this);
}

void MidiDevice::handleOpen() {
  if (_midiIn.openPort(_inputPortName)) {
    _midiIn.addListener(this);
  }
  _midiOut.openPort(_outputPortName);
  _params.enabled.setWithoutEventNotifications(true);
}

void MidiDevice::handleClose() {
  if (_midiIn.isOpen()) {
    _midiIn.removeListener(this);
    _midiIn.closePort();
  }
  if (_midiOut.isOpen()) {
    _midiOut.closePort();
  }
  _params.enabled.setWithoutEventNotifications(false);
}

void MidiDevice::newMidiMessage(ofxMidiMessage &message) {
  MidiReceivedEventArgs e(_id, message);
  messageReceived.notifyListeners(e);
}
