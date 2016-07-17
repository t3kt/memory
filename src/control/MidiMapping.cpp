//
//  MidiMapping.cpp
//  memory
//
//  Created by tekt on 7/16/16.
//
//

#include "MidiMapping.h"

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

Json MidiMappingSet::to_json() const {
  Json::array arr;
  for (const auto& mapping : _mappings) {
    arr.push_back(mapping.to_json());
  }
  return arr;
}

void MidiMappingSet::read_json(const Json& arr) {
  JsonUtil::assertHasType(arr, Json::ARRAY);
  _mappings.clear();
  for (const auto& val : arr.array_items()) {
    MidiMapping mapping;
    mapping.read_json(val);
    _mappings.push_back(mapping);
  }
}

void MidiMapping::output(std::ostream& os) const {
  os << "{key: " << _key
  << ", path: " << _path
  << "}";
}

void MidiMappingSet::output(std::ostream& os) const {
  os << "MidiMappingSet{";
  for (const auto& mapping : _mappings) {
    os << mapping << ", ";
  }
  os << "}";
}
