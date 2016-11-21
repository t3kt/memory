//
//  ControlValue.h
//

#pragma once

#include <iostream>
#include <ofMath.h>
#include <ofxTCommon.h>
#include <ofxTJsonIO.h>

enum class ControlValueMode {
  UNSPECIFIED,
  VALUE,
  RANDOM_RANGE,
  CHANCE,
};

std::ostream& operator<<(std::ostream& os,
                         const ControlValueMode& mode);

template<typename T>
class ControlValue
: public ofxTCommon::JsonReadable
, public ofxTCommon::JsonWritable
, public ofxTCommon::Outputable {
public:
  using Mode = ControlValueMode;

  ControlValue() {}

  ControlValue(Mode mode, T value0, T value1, float chance)
  : _mode(mode)
  , _value0(value0)
  , _value1(value1)
  , _chance(chance) { }

  T get(std::function<T()> getDefault) const {
    switch (_mode) {
      case Mode::VALUE:
        return _value0;
      case Mode::RANDOM_RANGE:
        return getRandomValue();
      case Mode::CHANCE:
        return getChanceValue();
      case Mode::UNSPECIFIED:
      default:
        return getDefault();
    }
  }

  T get() const {
    return get([]() { return T(); });
  }

  T get(T defaultValue) const {
    return get([=]() { return defaultValue; });
  }

  bool isSpecified() const { return _mode != Mode::UNSPECIFIED; }

  void readJson(const ofJson& obj) override {
    if (!obj.is_null()) {
      ofxTCommon::JsonUtil::assertIsObject(obj);
      auto val = obj.find("value");
      if (val != obj.end() && !val.value().is_null()) {
        _mode = Mode::VALUE;
        _value0 = ofxTCommon::JsonUtil::fromJson<T>(val.value());
        _value1 = _value0;
        _chance = 0;
        return;
      }
      auto range = obj.find("range");
      if (range != obj.end() && !range.value().is_null()) {
        ofxTCommon::JsonUtil::assertIsArray(range.value(), 2);
        _mode = Mode::RANDOM_RANGE;
        _value0 = ofxTCommon::JsonUtil::fromJson<T>(range.value()[0]);
        _value1 = ofxTCommon::JsonUtil::fromJson<T>(range.value()[1]);
        _chance = 0;
        return;
      }
      auto chance = obj.find("chance");
      auto options = obj.find("options");
      if (chance != obj.end() && options != obj.end()
          && !chance.value().is_null() && !options.value().is_null()) {
        ofxTCommon::JsonUtil::assertIsArray(options.value(), 2);
        _mode = Mode::CHANCE;
        _value0 = ofxTCommon::JsonUtil::fromJson<T>(options.value()[0]);
        _value1 = ofxTCommon::JsonUtil::fromJson<T>(options.value()[1]);
        _chance = chance.value();
        return;
      }
    }
    _mode = Mode::UNSPECIFIED;
    _value0 = T();
    _value1 = T();
    _chance = 0;
  }

  ofJson toJson() const override {
    switch (_mode) {
      case Mode::VALUE:
        return {
          {"value", ofxTCommon::JsonUtil::toJson(_value0)},
        };
      case Mode::RANDOM_RANGE:
        return {
          {"range", {
            ofxTCommon::JsonUtil::toJson(_value0),
            ofxTCommon::JsonUtil::toJson(_value1),
          }},
        };
      case Mode::CHANCE:
        return {
          {"chance", _chance},
          {"options", {
            ofxTCommon::JsonUtil::toJson(_value0),
            ofxTCommon::JsonUtil::toJson(_value1),
          }},
        };
      case Mode::UNSPECIFIED:
      default:
        return nullptr;
    }
  }

  std::string typeName() const override { return "ControlValue"; }

  void writeFieldTo(ofJson& obj, const std::string& name) const {
    if (isSpecified()) {
      obj[name] = toJson();
    }
  }
protected:
  void outputFields(std::ostream& os) const override {
    os << "mode: " << _mode;
    os << ", " << toJson().dump(0);
  }
private:

  T getRandomValue() const;

  T getChanceValue() const {
    if (ofRandomuf() < _chance) {
      return _value1;
    } else {
      return _value0;
    }
  }

  Mode _mode;
  T _value0;
  T _value1;
  float _chance;
};

namespace ControlValues {
  template<typename T>
  ControlValue<T> createValue(T value) {
    return ControlValue<T>(ControlValueMode::VALUE, value, value, 0);
  }
  template<typename T>
  ControlValue<T> createRandomRange(T value0, T value1) {
    return ControlValue<T>(ControlValueMode::RANDOM_RANGE,
                         value0, value1, 0);
  }
  template<typename T>
  ControlValue<T> createChance(T value0, T value1, float chance) {
    return ControlValue<T>(ControlValueMode::CHANCE,
                         value0, value1, chance);
  }

  template<typename T>
  ControlValue<T> createUnspecified() {
    return ControlValue<T>(ControlValueMode::UNSPECIFIED, T(), T(), 0);
  }
}
