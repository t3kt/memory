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
#include <vector>
#include "Common.h"
#include "Events.h"
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
  virtual void resetToDefault() = 0;
  virtual bool hasDefault() const = 0;
};

template<typename T>
class TParam
: public ofParameter<T>
, public TParamInfoBase {
public:
  TParam() {
    ofParameter<T>::addListener(this,
                                &TParam<T>::onChanged);
  }

  virtual ~TParam() override {
    ofParameter<T>::removeListener(this,
                                   &TParam<T>::onChanged);
  }

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

  const T& operator()() const {
    return ofParameter<T>::get();
  }

  bool hasDefault() const override {
    return _hasDefaultValue;
  }

  const T& getDefaultValue() const {
    return _defaultValue;
  }

  void clearDefaultValue() {
    _defaultValue = {};
    _hasDefaultValue = false;
  }

  void resetToDefault() override {
    if (hasDefault()) {
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

  TEvent<T&> changed;

  Json to_json() const override;

  void read_json(const Json& val) override;

  void readJsonField(const Json& obj) override {
    Json val = obj[getKey()];
    if (!val.is_null()) {
      read_json(val);
    } else if (hasDefault()) {
      ofParameter<T>::set(getDefaultValue());
    } else {
      throw JsonException("Required field missing: " + getKey());
    }
  }
  
private:
  void onChanged(T& value) {
    changed.notifyListeners(value);
  }

  std::string _key;
  bool _hasDefaultValue;
  T _defaultValue;
};

inline void toggleBoolParam(TParam<bool>& param) {
  param.set(!param.get());
}

class Params
: public ofParameterGroup
, public TParamInfoBase
, public NonCopyable {
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

  void add(Params& params) {
    ofParameterGroup::add(params);
    _paramBases.push_back(&params);
  }

  template<typename T>
  void add(TParam<T>& param) {
    ofParameterGroup::add(param);
    _paramBases.push_back(&param);
  }

  Json to_json() const override;

  void read_json(const Json& val) override;

  void readJsonField(const Json& obj) override;

  virtual void resetToDefault() override {
    for (auto param : _paramBases) {
      param->resetToDefault();
    }
  }

  virtual bool hasDefault() const override { return true; }
private:
  std::string _key;
  std::vector<TParamInfoBase*> _paramBases;
};

class ParamsWithEnabled : public Params {
public:
  ParamsWithEnabled() {
    add(enabled
        .setKey("enabled")
        .setName("Enabled")
        .setValueAndDefault(true));
  }

  void setEnabledValueAndDefault(bool val) {
    enabled.setValueAndDefault(val);
  }

  TParam<bool> enabled;
};

template<typename T>
class ValueRange : public Params {
public:
  ValueRange()
  : Params() {
    add(lowValue
        .setKey("low")
        .setName("Low"));
    add(highValue
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
    lowValue.setKey(lowKey);
    highValue.setKey(highKey);
    return *this;
  }

  ValueRange<T>& setParamNames(std::string lowName, std::string highName) {
    lowValue.setName(lowName);
    highValue.setName(highName);
    return *this;
  }

  ValueRange<T>& setParamValues(T low, T high) {
    lowValue.set(low);
    highValue.set(high);
    return *this;
  }

  ValueRange<T>& setParamValuesAndDefaults(T low, T high) {
    lowValue.setValueAndDefault(low);
    highValue.setValueAndDefault(high);
    return *this;
  }

  ValueRange<T>& setParamRanges(T minVal, T maxVal) {
    lowValue.setMin(minVal);
    lowValue.setMax(maxVal);
    highValue.setMin(minVal);
    highValue.setMax(maxVal);
    return *this;
  }

  T getLerped(float amount) const {
    return getInterpolated(lowValue(), highValue(), amount);
  }

  TParam<T> lowValue;
  TParam<T> highValue;
};

using FloatValueRange = ValueRange<float>;

#endif /* Params_h */
