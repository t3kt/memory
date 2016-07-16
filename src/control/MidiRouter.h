//
//  MidiRouter.h
//  memory
//
//  Created by tekt on 7/15/16.
//
//

#ifndef MidiRouter_h
#define MidiRouter_h

#include <array>
#include <memory>
#include <ofEventUtils.h>
#include <ofxMidiFighterTwister.h>
#include "AppParameters.h"
#include "Events.h"
#include "MidiDevice.h"
#include "Params.h"

class MidiMapping {
public:
  MidiMappingKey key;
  const std::string target;
};

class AbstractMidiBinding;

class MidiRouter {
public:
  using BindingArray = std::array<std::shared_ptr<AbstractMidiBinding>, 128>;

  MidiRouter(MemoryAppParameters& appParams)
  : _appParams(appParams) { }

  void setup();

  void attachTo(std::shared_ptr<ofxMidiFighterTwister> twister);
  void detachFrom(std::shared_ptr<ofxMidiFighterTwister> twister);
private:
  void receiveValue(int cc, int value);

  void onTwisterEncoder(ofxMidiFighterTwister::EncoderEventArgs& event);

  MemoryAppParameters& _appParams;
  BindingArray _bindings;
};

#endif /* MidiRouter_h */
