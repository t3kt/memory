//
//  MidiController.cpp
//  memory
//
//  Created by tekt on 7/15/16.
//
//

#include "AppParameters.h"
#include "Common.h"
#include "MidiController.h"
#include "MidiRouter.h"

MidiController::MidiController(MemoryAppParameters& appParams)
: _appParams(appParams)
, _params(appParams.core.midi) { }

void MidiController::setup() {
  _router = std::make_shared<MidiRouter>(_appParams);
  _params.enabledChanged += [&](ValueEventArgs<bool>) {
    initOrDestroyTwister();
  };
  _params.twisterEnabledChanged += [&](ValueEventArgs<bool>) {
    initOrDestroyTwister();
  };
  initOrDestroyTwister();
}

void MidiController::initOrDestroyTwister() {
  if (_params.enabled() && _params.twisterEnabled()) {
    initializeTwister();
  } else {
    destroyTwister();
  }
}

void MidiController::initializeTwister() {
  if (_twister) {
    destroyTwister();
  }
  _twister = std::make_shared<ofxMidiFighterTwister>();
  _twister->setup();
  _router->attachTo(_twister);
}

void MidiController::destroyTwister() {
  if (!_twister) {
    return;
  }
  _router->detachFrom(_twister);
  _twister.reset();
}

void MidiController::update() {
  if (_twister) {
    _twister->update();
  }
}
