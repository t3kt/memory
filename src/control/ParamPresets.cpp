//
//  ParamPresets.cpp
//  memory
//
//  Created by tekt on 10/11/16.
//
//

#include "../control/ParamPresets.h"

static ConstParamPredicate presetPredicate =
[](const TParamBase& param) {
  return param.supportsPresets();
};

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
  _values = params.toFilteredJson(presetPredicate);
}

void ParamPreset::applyToParams(Params &params) const {
  params.readFilteredJson(_values, presetPredicate);
}

static Json stripUnsupported(const Json& obj, const Params& params) {
  if (!params.supportsPresets() || !obj.is_object()) {
    return nullptr;
  }
  Json::object result;
  for (const auto& entry : obj.object_items()) {
    auto param = params.findKey(entry.first);
    if (param && param->supportsPresets()) {
      if (param->isGroup()) {
        auto group = dynamic_cast<const Params*>(param);
        if (group) {
          auto val = stripUnsupported(entry.second, *group);
          if (!val.is_null()) {
            result.insert(std::make_pair(entry.first, val));
          }
        }
      } else {
        result.insert(entry);
      }
    }
  }
  return result;
}

void ParamPreset::stripUnsupportedParams(const Params &params) {
  auto values = stripUnsupported(_values, params);
  if (values.is_object()) {
    _values = values;
  } else {
    _values = Json::object();
  }
}
