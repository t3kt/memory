//
//  ParamPresets.cpp
//  memory
//
//  Created by tekt on 10/11/16.
//
//

#include "../control/ParamPresets.h"

using namespace ofxTCommon;

static ConstParamPredicate presetPredicate =
[](const TParamBase& param) {
  return param.supportsPresets();
};

ofJson ParamPreset::toJson() const {
  return {
    {"name", _name},
    {"params", _values},
  };
}

void ParamPreset::readJson(const ofJson &obj) {
  JsonUtil::assertIsObject(obj);
  _name = JsonUtil::fromJsonField<std::string>(obj, "name");
  auto vals = obj["params"];
  JsonUtil::assertIsObject(vals);
  _values = vals;
}

void ParamPreset::captureParams(const Params &params) {
  _values = params.toFilteredJson(presetPredicate);
}

void ParamPreset::applyToParams(Params &params) const {
  params.readFilteredJson(_values, presetPredicate);
}

static ofJson stripUnsupported(const ofJson& obj,
                               const Params& params) {
  if (!params.supportsPresets() || !obj.is_object()) {
    return nullptr;
  }
  auto result = ofJson::object();
  for (auto iter = obj.begin(); iter != obj.end(); ++iter) {
    auto param = params.findKey(iter.key());
    if (param && param->supportsPresets()) {
      if (param->isGroup()) {
        auto group = dynamic_cast<const Params*>(param);
        if (group) {
          auto val = stripUnsupported(iter.value(), *group);
          if (!val.is_null()) {
            result[iter.key()] = val;
          }
        }
      } else {
        result[iter.key()] = iter.value();
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
    _values = ofJson::object();
  }
}
