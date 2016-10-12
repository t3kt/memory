//
//  ParamPresets.cpp
//  memory
//
//  Created by tekt on 10/11/16.
//
//

#include "../control/ParamPresets.h"

Json ParamPreset::to_json() const {
  return Json::object {
    {"name", _name},
    {"values", _values},
  };
}

void ParamPreset::read_json(const Json &obj) {
  JsonUtil::assertHasType(obj, Json::OBJECT);
  _name = JsonUtil::fromJsonField<std::string>(obj, "name");
  auto vals = obj["values"];
  JsonUtil::assertHasType(vals, Json::OBJECT);
  _values = vals;
}
