//
//  Params.h
//  memory-prototype-2
//
//  Created by tekt on 6/24/16.
//
//

#ifndef Params_h
#define Params_h

#include <memory>
#include <ofParameterGroup.h>
#include <ofUtils.h>
#include <ofxControlParameter.h>
#include <string>
#include <typeinfo>
#include <vector>
#include "Common.h"
#include "Events.h"
#include "JsonIO.h"

class TParamBase
: public JsonReadable
, public JsonWritable {
public:
  virtual std::string getKey() const = 0;
  virtual Json::object::value_type toJsonField() const {
    return { getKey(), to_json() };
  }
  virtual void readJsonField(const Json& obj) = 0;
  virtual void resetToDefault() = 0;
  virtual bool hasDefault() const = 0;
  virtual bool isGroup() const = 0;

  virtual std::string asString() const = 0;
  virtual const std::type_info& getTypeInfo() const = 0;
};

template<typename T, typename ParT>
class TTypedParamBase
: public ofParameter<T>
, public TParamBase {
public:
  using CtrlParT = ofxControlParameter<T>;

  TTypedParamBase() {
    ofParameter<T>::addListener(this,
                                &TTypedParamBase::onChanged);
  }

  virtual ~TTypedParamBase() override {
    ofParameter<T>::removeListener(this,
                                   &TTypedParamBase::onChanged);
  }

  ParT& setKey(std::string key) {
    _key = key;
    return selfRef();
  }

  ParT& setName(std::string name) {
    ofParameter<T>::setName(name);
    return selfRef();
  }

  ParT& setRange(T minValue, T maxValue) {
    ofParameter<T>::setMin(minValue);
    ofParameter<T>::setMax(maxValue);
    return selfRef();
  }

  ParT& setDefaultValue(T defaultValue) {
    _defaultValue = defaultValue;
    _hasDefaultValue = true;
    return selfRef();
  }

  ParT& setValueAndDefault(T value) {
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

  bool isGroup() const override { return false; }

  const std::type_info& getTypeInfo() const override {
    return typeid(T);
  }

  std::string asString() const override { return ofParameter<T>::toString(); }

  TEvent<T&> changed;

  Json to_json() const override {
    return JsonUtil::toJson(ofParameter<T>::get());
  }

  void read_json(const Json& val) override {
    this->set(JsonUtil::fromJson<T>(val));
  }

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

  CtrlParT* getControlPar() {
    if (!_controlPar) {
      initializeControlPar();
    }
    return _controlPar.get();
  }
  
protected:
  virtual ParT& selfRef() = 0;

  virtual void initializeControlPar() {
    _controlParRelayValue = ofParameter<T>::get();
    _controlPar =
    std::make_shared<CtrlParT>(ofParameter<T>::getName(),
                               &_controlParRelayValue,
                               ofParameter<T>::getMin(),
                               ofParameter<T>::getMax());
  }

  std::shared_ptr<CtrlParT> _controlPar;

private:
  void onChanged(T& value) {
    changed.notifyListeners(value);
    _controlParRelayValue = value;
    if (_controlPar) {
      _controlPar->valueChanged();
    }
  }

  T _controlParRelayValue;
  std::string _key;
  bool _hasDefaultValue;
  T _defaultValue;
};

template<typename T>
class TParam
: public TTypedParamBase<T, TParam<T>> {
public:

  void setNormalizedValue(float normVal) {
    ofParameter<T>::set(getInterpolated(ofParameter<T>::getMin(),
                                        ofParameter<T>::getMax(),
                                        normVal));
  }

protected:
  TParam<T>& selfRef() override { return *this; }
};

template<>
class TParam<bool>
: public TTypedParamBase<bool, TParam<bool>> {
public:

  void setNormalizedValue(float normVal) {
    ofParameter<bool>::set(normVal > 0);
  }

  void toggle() {
    ofParameter<bool>::set(!ofParameter<bool>::get());
  }

protected:
  TParam<bool>& selfRef() override { return *this; }
};

class Params
: public ofParameterGroup
, public TParamBase
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

  bool isGroup() const override { return true; }
  const std::type_info& getTypeInfo() const override {
    return typeid(Params);
  }

  std::string asString() const override { return ofParameterGroup::toString(); }

  TParamBase* findKey(const std::string& key);

  TParamBase* lookupPath(const std::string& path);
private:
  std::string _key;
  std::vector<TParamBase*> _paramBases;
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
