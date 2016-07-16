//
//  MidiRouter.cpp
//  memory
//
//  Created by tekt on 7/15/16.
//
//

#include "Common.h"
#include "MidiRouter.h"

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

void MidiRouter::attachTo(std::shared_ptr<ofxMidiFighterTwister> twister) {
  ofAddListener(twister->eventEncoder,
                this,
                &MidiRouter::onTwisterEncoder);
}

void MidiRouter::detachFrom(std::shared_ptr<ofxMidiFighterTwister> twister) {
  ofRemoveListener(twister->eventEncoder,
                   this,
                   &MidiRouter::onTwisterEncoder);
}

void MidiRouter::onTwisterEncoder(ofxMidiFighterTwister::EncoderEventArgs& event) {
  switch (event.ID) {
    case 0:
      applyMidiValueToParam(_appParams.core.clock._rate, event.value);
      break;
    case 64:
      applyMidiValueToParam(_appParams.core.clock._paused, event.value);
      break;
  }
}
