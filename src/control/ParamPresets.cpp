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
    {"params", _values},
  };
}

void ParamPreset::read_json(const Json &obj) {
  JsonUtil::assertHasType(obj, Json::OBJECT);
  _name = JsonUtil::fromJsonField<std::string>(obj, "name");
  auto vals = obj["params"];
  JsonUtil::assertHasType(vals, Json::OBJECT);
  _values = vals;
}

void ParamPreset::captureParams(const Params &params) {
  _values = params.to_json();
}

void ParamPreset::applyToParams(Params &params) const {
  params.read_json(_values);
}
