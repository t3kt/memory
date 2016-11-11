//
//  MidiMapping.cpp
//  memory
//
//  Created by tekt on 7/16/16.
//
//

#include "../control/MidiMapping.h"

using namespace ofxTCommon;

ofJson MidiMapping::toJson() const {
  return {
    {"key", _key.toJson()},
    {"path", _path},
  };
}

void MidiMapping::readJson(const ofJson &obj) {
  JsonUtil::assertIsObject(obj);
  _key.readJson(obj["key"]);
  _path = JsonUtil::fromJson<std::string>(obj["path"]);
}

void MidiMapping::outputFields(std::ostream& os) const {
  os << "key: " << _key
  << ", path: " << _path;
}
