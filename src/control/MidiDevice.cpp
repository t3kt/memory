//
//  MidiDevice.cpp
//  memory
//
//  Created by tekt on 7/15/16.
//
//

#include <algorithm>
#include "MidiDevice.h"


static MidiDeviceId nextId() {
  static MidiDeviceId lastId = 0;
  return ++lastId;
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
      handleClose(true);
    }
  }, this);
  if (params.enabled) {
    handleOpen();
  }
}

template<typename M>
bool tryOpenPort(M& midi, const std::string& portName) {
  auto portNames = M::getPortList();
  if (std::find(portNames.begin(), portNames.end(), portName) == portNames.end()) {
    return false;
  }
  return midi.openPort(portName);
}

void MidiDevice::handleOpen() {
  auto hasIn = tryOpenPort(_midiIn, _inputPortName);
  auto hasOut = tryOpenPort(_midiOut, _outputPortName);
  if (hasIn) {
    _midiIn.addListener(this);
  }
  if (hasIn || hasOut) {
    _params.enabled.setWithoutEventNotifications(true);
  } else {
    close();
  }
}

void MidiDevice::handleClose(bool updateParams) {
  if (_midiIn.isOpen()) {
    _midiIn.removeListener(this);
    _midiIn.closePort();
  }
  if (_midiOut.isOpen()) {
    _midiOut.closePort();
  }
  if (updateParams) {
    _params.enabled.setWithoutEventNotifications(false);
  }
}

void MidiDevice::newMidiMessage(ofxMidiMessage &message) {
  MidiReceivedEventArgs e(_id, message);
  messageReceived.notifyListeners(e);
}
