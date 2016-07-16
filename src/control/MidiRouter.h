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
#include <vector>
#include "AppParameters.h"
#include "Events.h"
#include "JsonIO.h"
#include "MidiDevice.h"
#include "Params.h"

class MidiMapping {
public:
  MidiMapping() {}
  MidiMapping(MidiMappingKey key, std::string path)
  : _key(key)
  , _path(path) {}

  Json to_json() const;
  void read_json(const Json& obj);

private:
  MidiMappingKey _key;
  std::string _path;
};

class MidiMappingSet {
public:
  using Storage = std::vector<MidiMapping>;
  using iterator = Storage::iterator;
  using const_iterator = Storage::const_iterator;

  Json to_json() const;
  void read_json(const Json& obj);

  iterator begin() { return _mappings.begin(); }
  iterator end() { return _mappings.end(); }
  const_iterator begin() const { return _mappings.begin(); }
  const_iterator end() const { return _mappings.end(); }
private:
  Storage _mappings;
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
