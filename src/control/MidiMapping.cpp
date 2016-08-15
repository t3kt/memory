//
//  MidiMapping.cpp
//  memory
//
//  Created by tekt on 7/16/16.
//
//

#include "../control/MidiMapping.h"

Json MidiMapping::to_json() const {
  return Json::object {
    {"key", _key},
    {"path", _path},
  };
}

void MidiMapping::read_json(const Json &obj) {
  JsonUtil::assertHasType(obj, Json::OBJECT);
  _key.read_json(obj["key"]);
  _path = JsonUtil::fromJson<std::string>(obj["path"]);
}

void MidiMapping::outputFields(std::ostream& os) const {
  os << "key: " << _key
  << ", path: " << _path;
}
