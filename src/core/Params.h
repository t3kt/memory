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
#include <ofxGuiGroup.h>
#include "Common.h"
#include "JsonIO.h"

class ofxDatGuiFolder;

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
  static Json::Type jsonType;

  TParam<T>& setKey(std::string key) {
    _key = key;
    return *this;
  }

  TParam<T>& setName(std::string name) {
    ofParameter<T>::setName(name);
    return *this;
  }

  TParam<T>& setRange(T minValue, T maxValue) {
    setMin(minValue);
    setMax(maxValue);
    return *this;
  }

  TParam<T>& setDefaultValue(T defaultValue) {
    _defaultValue = defaultValue;
    _hasDefaultValue = true;
    return *this;
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

  std::string getKey() const override {
    if (_key.empty()) {
      return ofParameter<T>::getEscapedName();
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

class ParamsGui {
public:
  virtual void setup() {}
  virtual ofxDatGuiFolder* getGuiFolder() = 0;
};

class Params
: public ofParameterGroup
, public TParamInfoBase {
public:
  Params() {}
  explicit Params(std::string label) {
    setName(label);
  }

  std::string getKey() const override {
    if (_key.empty()) {
      return ofParameterGroup::getEscapedName();
    } else {
      return _key;
    }
  }

  virtual Json to_json() const override = 0;

  virtual void read_json(const Json& val) override = 0;

  void readJsonField(const Json& obj) override;
  
  virtual void initPanel(ofxGuiGroup& panel) {}

  ParamsGui* getGui() {
    if (_gui == nullptr) {
      _gui = createGui();
      if (_gui != nullptr) {
        _gui->setup();
      }
    }
    return _gui;
  }
protected:
  virtual void resetToDefaults() {}
  virtual bool hasDefaults() const { return false; }
  virtual ParamsGui* createGui() { return nullptr; }
private:
  std::string _key;
  ParamsGui* _gui;
};

template<typename T>
class ValueRange : public Params {
public:
  explicit ValueRange(std::string name)
  : Params(name) {
    lowValue.setName("Low");
    highValue.setName("High");
    add(lowValue);
    add(highValue);
  }

  ValueRange<T>& setNames(std::string lowName, std::string highName) {
    lowValue.setName(lowName);
    highValue.setName(highName);
    return *this;
  }

  ValueRange<T>& set(T low, T high) {
    lowValue.set(low);
    highValue.set(high);
    return *this;
  }

  ValueRange<T>& setParamRange(T minVal, T maxVal) {
    lowValue.setMin(minVal);
    lowValue.setMax(maxVal);
    highValue.setMin(minVal);
    highValue.setMax(maxVal);
    return *this;
  }

  T getLerped(float amount) const {
    return getInterpolated(lowValue.get(), highValue.get(), amount);
  }

  Json to_json() const override;

  void read_json(const Json& val) override;
  
  TParam<T> lowValue;
  TParam<T> highValue;

protected:
  ParamsGui* createGui() override;
};

using FloatValueRange = ValueRange<float>;

#endif /* Params_h */
