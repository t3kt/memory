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
#include <unordered_map>
#include "AppParameters.h"
#include "Events.h"
#include "JsonIO.h"
#include "MidiDevice.h"
#include "MidiMapping.h"
#include "Params.h"

class AbstractMidiBinding;

class MidiRouter {
public:
  using BindingMap = std::unordered_map<MidiMappingKey, std::shared_ptr<AbstractMidiBinding>>;

  MidiRouter(MemoryAppParameters& appParams)
  : _appParams(appParams) { }

  void setup();

  void attachTo(std::shared_ptr<ofxMidiFighterTwister> twister);
  void detachFrom(std::shared_ptr<ofxMidiFighterTwister> twister);
private:
  void loadMappings();
  void addBinding(const MidiMapping& mapping);

  void receiveValue(int cc, int value);

  void onTwisterEncoder(ofxMidiFighterTwister::EncoderEventArgs& event);

  MemoryAppParameters& _appParams;
  MidiMappingSet _mappings;
  BindingMap _bindings;
};

#endif /* MidiRouter_h */
