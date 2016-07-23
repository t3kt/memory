//
//  OscController.cpp
//  memory
//
//  Created by tekt on 7/22/16.
//
//

#include "AppParameters.h"
#include "OscController.h"

class AbstractOscBinding {
public:
protected:
};

OscController::OscController(MemoryAppParameters& appParams)
: _params(appParams.core.osc)
, _appParams(appParams) {
  TEvent<bool>::VoidFunctor handler = [this]() {
    handleOpen(); // closes if disabled
  };
  _params.enabled.changed.addVoidListener(handler, this);
  _params.inputEnabled.changed.addVoidListener(handler, this);
  _params.outputEnabled.changed.addVoidListener(handler, this);
  handleOpen();
}

OscController::~OscController() {
  handleClose(false);
  _params.enabled.changed.removeListeners(this);
  _params.inputEnabled.changed.removeListeners(this);
  _params.outputEnabled.changed.removeListeners(this);
}

void OscController::handleOpen() {
  handleClose(false);
  if (!_params.enabled.get()) {
    _params.enabled.set(false);
  }
  if (_params.inputEnabled.get()) {
    _receiver = std::make_shared<ofxOscReceiver>();
    _receiver->setup(_params.inputPort.get());
  }
  if (_params.outputEnabled.get()) {
    std::string host = _params.outputHost.get();
    if (host.empty()) {
      host = _params.outputHost.getDefaultValue();
    }
    _sender = std::make_shared<ofxOscSender>();
    _sender->setup(host, _params.outputPort.get());
  }
}

void OscController::handleClose(bool updateParams) {
  _sender.reset();
  _receiver.reset();
  if (updateParams) {
    _params.enabled.setWithoutEventNotifications(false);
  }
}

void OscController::queueOutputMessage(const ofxOscMessage &message) {
  if (_receiving) {
    return;
  }
  _outputBundle.addMessage(message);
}

void OscController::update() {
  if (_sender && _outputBundle.getMessageCount() > 0) {
    _sender->sendBundle(_outputBundle);
  }
  _outputBundle.clear();
  if (_receiver) {
    _receiving = true;
    auto receiver = *_receiver;
    ofxOscMessage message;
    while (receiver.getNextMessage(message)) {
      handleMessage(message);
    }
    _receiving = false;
  }
}
