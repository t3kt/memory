//
//  Params.h
//  memory-prototype-2
//
//  Created by tekt on 6/24/16.
//
//

#ifndef Params_h
#define Params_h

#include <string>
#include <ofParameterGroup.h>
#include <ofUtils.h>
#include "Common.h"
#include "JsonIO.h"

class TParamInfoBase {
public:
  virtual std::string getKey() const = 0;
  virtual Json to_json() const = 0; // json11 library requires this naming
  virtual Json::object::value_type toJsonField() const {
    return { getKey(), to_json() };
  }
  virtual void read_json(const Json& val) = 0;
  virtual void readJsonField(const Json& obj) = 0;
};

template<typename T>
class TParam
: public ofParameter<T>
, public TParamInfoBase {
public:
  TParam<T>& setKey(std::string key) {
    _key = key;
    return *this;
  }

  TParam<T>& setName(std::string name) {
    ofParameter<T>::setName(name);
    return *this;
  }

  TParam<T>& setRange(T minValue, T maxValue) {
    ofParameter<T>::setMin(minValue);
    ofParameter<T>::setMax(maxValue);
    return *this;
  }

  TParam<T>& setDefaultValue(T defaultValue) {
    _defaultValue = defaultValue;
    _hasDefaultValue = true;
    return *this;
  }

  TParam<T>& setValueAndDefault(T value) {
    ofParameter<T>::set(value);
    return setDefaultValue(value);
  }

  bool hasDefaultValue() const {
    return _hasDefaultValue;
  }

  const T& getDefaultValue() const {
    return _defaultValue;
  }

  void clearDefaultValue() {
    _defaultValue = {};
    _hasDefaultValue = false;
  }

  void resetToDefault() {
    if (hasDefaultValue()) {
      ofParameter<T>::set(getDefaultValue());
    }
  }

  std::string getKey() const override {
    if (_key.empty()) {
      return ofToLower(ofParameter<T>::getEscapedName());
    } else {
      return _key;
    }
  }

  Json to_json() const override;

  void read_json(const Json& val) override;

  void readJsonField(const Json& obj) override {
    Json val = obj[getKey()];
    if (!val.is_null()) {
      read_json(val);
    } else if (hasDefaultValue()) {
      ofParameter<T>::set(getDefaultValue());
    } else {
      throw JsonException("Required field missing: " + getKey());
    }
  }
  
private:
  std::string _key;
  bool _hasDefaultValue;
  T _defaultValue;
};

class Params
: public ofParameterGroup
, public TParamInfoBase {
public:
  Params() {}
  Params(std::string key, std::string label) {
    setKey(key);
    setName(label);
  }

  std::string getKey() const override {
    if (_key.empty()) {
      return ofToLower(ofParameterGroup::getEscapedName());
    } else {
      return _key;
    }
  }

  Params& setKey(std::string key) {
    _key = key;
    return *this;
  }

  Params& setName(std::string name) {
    ofParameterGroup::setName(name);
    return *this;
  }

  virtual Json to_json() const override = 0;

  virtual void read_json(const Json& val) override = 0;

  void readJsonField(const Json& obj) override;

  virtual void resetToDefaults() {}
  virtual bool hasDefaults() const { return false; }
private:
  std::string _key;
};

template<typename T>
class ValueRange : public Params {
public:
  ValueRange()
  : Params() {
    add(_lowValue
        .setKey("low")
        .setName("Low"));
    add(_highValue
        .setKey("high")
        .setName("High"));
  }

  ValueRange<T>& setKey(std::string key) {
    Params::setKey(key);
    return *this;
  }

  ValueRange<T>& setName(std::string name) {
    Params::setName(name);
    return *this;
  }

  ValueRange<T>& setParamKeys(std::string lowKey, std::string highKey) {
    _lowValue.setKey(lowKey);
    _highValue.setKey(highKey);
    return *this;
  }

  ValueRange<T>& setParamNames(std::string lowName, std::string highName) {
    _lowValue.setName(lowName);
    _highValue.setName(highName);
    return *this;
  }

  ValueRange<T>& setParamValues(T low, T high) {
    _lowValue.set(low);
    _highValue.set(high);
    return *this;
  }

  ValueRange<T>& setParamValuesAndDefaults(T low, T high) {
    _lowValue.setValueAndDefault(low);
    _highValue.setValueAndDefault(high);
    return *this;
  }

  ValueRange<T>& setParamRanges(T minVal, T maxVal) {
    _lowValue.setMin(minVal);
    _lowValue.setMax(maxVal);
    _highValue.setMin(minVal);
    _highValue.setMax(maxVal);
    return *this;
  }

  T getLerped(float amount) const {
    return getInterpolated(lowValue(), highValue(), amount);
  }

  Json to_json() const override;

  void read_json(const Json& val) override;

  virtual void resetToDefaults() override {
    _lowValue.resetToDefault();
    _highValue.resetToDefault();
  }
  virtual bool hasDefaults() const override { return true; }

  const T& lowValue() const { return _lowValue.get(); }
  const T& highValue() const { return _highValue.get(); }

protected:
  TParam<T> _lowValue;
  TParam<T> _highValue;
};

using FloatValueRange = ValueRange<float>;

#endif /* Params_h */
