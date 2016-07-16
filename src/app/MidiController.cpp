//
//  MidiController.cpp
//  memory
//
//  Created by tekt on 7/15/16.
//
//

#include <ofEventUtils.h>
#include <ofxMidiFighterTwister.h>
#include "AppParameters.h"
#include "Common.h"
#include "MidiController.h"

template<typename T>
static void applyMidiValueToParam(TParam<T>& param, int value) {
  float percent = value / 128.0f;
  param.set(getInterpolated(param.getMin(),
                            param.getMax(),
                            percent));
}

template<>
void applyMidiValueToParam(TParam<bool>& param, int value) {
  param.set(value > 0);
}

class MidiRouter {
public:
  MidiRouter(MemoryAppParameters& appParams)
  : _appParams(appParams) { }

  void attachTo(std::shared_ptr<ofxMidiFighterTwister> twister) {
    ofAddListener(twister->eventEncoder,
                  this,
                  &MidiRouter::onTwisterEncoder);
  }
  void detachFrom(std::shared_ptr<ofxMidiFighterTwister> twister) {
    ofRemoveListener(twister->eventEncoder,
                     this,
                     &MidiRouter::onTwisterEncoder);
  }
private:
  void onTwisterEncoder(ofxMidiFighterTwister::EncoderEventArgs& event) {
    switch (event.ID) {
      case 0:
        applyMidiValueToParam(_appParams.core.clock._rate, event.value);
        break;
      case 64:
        applyMidiValueToParam(_appParams.core.clock._paused, event.value);
        break;
    }
  }

  MemoryAppParameters& _appParams;
};

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
