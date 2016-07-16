//
//  MidiRouter.cpp
//  memory
//
//  Created by tekt on 7/15/16.
//
//

#include "Common.h"
#include "MidiRouter.h"

class AbstractMidiMapping {
public:
  virtual void receiveValue(int value) = 0;
};

template<typename T>
class MidiMapping
: public AbstractMidiMapping {
public:
  MidiMapping(TParam<T>& param)
  : _param(param) {}

  void receiveValue(int value) override {
    float percent = value / 128.0f;
    _param.set(getInterpolated(_param.getMin(),
                               _param.getMax(),
                               percent));
  }
protected:
  TParam<T>& _param;
};

template<>
void MidiMapping<bool>::receiveValue(int value) {
  _param.set(value > 0);
}

template<typename T>
void addMapping(MidiRouter::MappingArray& mappings,
                int cc,
                TParam<T>& param) {
  mappings[cc] = std::make_shared<MidiMapping<T>>(param);
}


void MidiRouter::setup() {
  addMapping(_mappings, 0, _appParams.core.clock._rate);
  addMapping(_mappings, 64, _appParams.core.clock._paused);
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
  auto& mapping = _mappings[cc];
  if (mapping) {
    mapping->receiveValue(value);
  }
}
