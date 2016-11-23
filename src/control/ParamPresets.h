//
//  ParamPresets.h
//

#pragma once

#include <memory>
#include <ofxTJsonIO.h>
#include <utility>
#include "../control/Params.h"

class ParamPreset
: public ofxTCommon::JsonReadable
, public ofxTCommon::JsonWritable {
public:
  ParamPreset()
  : _values(ofJson::object()) {}

  const std::string& name() const { return _name; }
  void setName(const std::string& name) { _name = name; }

  void readJson(const ofJson& obj) override;
  ofJson toJson() const override;

  void captureParams(const Params& params);
  void applyToParams(Params& params) const;

  bool containsKey(const std::string& key) const {
    const auto iter = _values.find(key);
    return iter != _values.end() && !iter.value().is_null();
  }
  const ofJson& getJsonValue(const std::string& key) const {
    return _values[key];
  }

  template<typename T>
  T getValue(const std::string& key) const {
    const auto& jsonVal = getJsonValue(key);
    return ofxTCommon::JsonUtil::fromJson<T>(jsonVal);
  }

  const ofJson& values() const { return _values; }

  void stripUnsupportedParams(const Params& params);

private:
  std::string _name;
  ofJson _values;
};

using PresetPtr = std::shared_ptr<ParamPreset>;
