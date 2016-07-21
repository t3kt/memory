//
//  ControlMapping.cpp
//  memory
//
//  Created by tekt on 7/16/16.
//
//

#include "ControlMapping.h"

Json ControlMapping::to_json() const {
  return Json::object {
    {"key", _key},
    {"path", _path},
  };
}

void ControlMapping::read_json(const Json &obj) {
  JsonUtil::assertHasType(obj, Json::OBJECT);
  _key.read_json(obj["key"]);
  _path = JsonUtil::fromJson<std::string>(obj["path"]);
}

Json ControlMappingSet::to_json() const {
  Json::array arr;
  for (const auto& mapping : _mappings) {
    arr.push_back(mapping.to_json());
  }
  return arr;
}

void ControlMappingSet::read_json(const Json& arr) {
  JsonUtil::assertHasType(arr, Json::ARRAY);
  _mappings.clear();
  for (const auto& val : arr.array_items()) {
    ControlMapping mapping;
    mapping.read_json(val);
    _mappings.push_back(mapping);
  }
}

void ControlMapping::outputFields(std::ostream& os) const {
  os << "key: " << _key
  << ", path: " << _path;
}

void ControlMappingSet::outputFields(std::ostream& os) const {
  for (const auto& mapping : _mappings) {
    os << mapping << ", ";
  }
}
