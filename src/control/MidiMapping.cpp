//
//  MidiMapping.cpp
//  memory
//
//  Created by tekt on 7/16/16.
//
//

#include "../control/MidiMapping.h"

ofJson MidiMapping::toJson() const {
  return {
    {"key", _key.toJson()},
    {"path", _path},
  };
}

void MidiMapping::readJson(const ofJson &obj) {
  ofxTCommon::JsonUtil::assertHasType(obj, ofJson::value_t::object);
  _key.readJson(obj["key"]);
  _path = ofxTCommon::JsonUtil::fromJson<std::string>(obj["path"]);
}

void MidiMapping::outputFields(std::ostream& os) const {
  os << "key: " << _key
  << ", path: " << _path;
}
