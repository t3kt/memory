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
#include "Params.h"

class AbstractMidiMapping;

class MidiRouter {
public:
  using MappingArray = std::array<std::shared_ptr<AbstractMidiMapping>, 128>;

  MidiRouter(MemoryAppParameters& appParams)
  : _appParams(appParams) { }

  void setup();

  void attachTo(std::shared_ptr<ofxMidiFighterTwister> twister);
  void detachFrom(std::shared_ptr<ofxMidiFighterTwister> twister);
private:
  void receiveValue(int cc, int value);

  void onTwisterEncoder(ofxMidiFighterTwister::EncoderEventArgs& event);

  MemoryAppParameters& _appParams;
  MappingArray _mappings;
};

#endif /* MidiRouter_h */
