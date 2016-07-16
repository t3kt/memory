//
//  MidiRouter.cpp
//  memory
//
//  Created by tekt on 7/15/16.
//
//

#include "Common.h"
#include "MidiRouter.h"

class MappingKey {
public:
  MidiDeviceId deviceId;

};

class AbstractMidiBinding {
public:
  virtual void receiveValue(int value) = 0;
};

template<typename T>
class MidiBinding
: public AbstractMidiBinding {
public:
  MidiBinding(TParam<T>& param)
  : _param(param) {}

  void receiveValue(int value) override {
    _param.setNormalizedValue(value / 128.0f);
  }
protected:
  TParam<T>& _param;
};

void MidiRouter::setup() {
//  addMapping(_bindings, 0, _appParams.core.clock.rate);
//  addMapping(_bindings, 64, _appParams.core.clock.paused);
  loadMappings();
}

void MidiRouter::loadMappings() {
  //...
}

void MidiRouter::addBinding(const MidiMapping& mapping) {
  //...
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
  receiveValue(event.ID, event.value);
}

void MidiRouter::receiveValue(int cc, int value) {
//  auto& binding = _bindings[cc];
//  if (binding) {
//    binding->receiveValue(value);
//  }
  //...
}
