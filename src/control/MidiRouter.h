//
//  MidiRouter.h
//  memory
//
//  Created by tekt on 7/15/16.
//
//

#ifndef MidiRouter_h
#define MidiRouter_h

#include <memory>
#include <ofEventUtils.h>
#include <ofxMidiFighterTwister.h>
#include "AppParameters.h"
#include "Events.h"

class MidiRouter {
public:
  MidiRouter(MemoryAppParameters& appParams)
  : _appParams(appParams) { }

  void attachTo(std::shared_ptr<ofxMidiFighterTwister> twister);
  void detachFrom(std::shared_ptr<ofxMidiFighterTwister> twister);
private:
  void onTwisterEncoder(ofxMidiFighterTwister::EncoderEventArgs& event);

  MemoryAppParameters& _appParams;
};

#endif /* MidiRouter_h */
