//
//  SceneValue.h
//  memory
//

#pragma once

#include <iostream>
#include <ofMath.h>
#include <ofxTCommon.h>
#include <ofxTJsonIO.h>

enum class SceneValueMode {
  NONE,
  VALUE,
  RANDOM_RANGE,
  CHANCE,
};

std::ostream& operator<<(std::ostream& os,
                         const SceneValueMode& mode);

template<typename T>
class SceneValue
: public ofxTCommon::JsonReadable
, public ofxTCommon::JsonWritable
, public ofxTCommon::Outputable {
public:
  using Mode = SceneValueMode;

  SceneValue() {}

  SceneValue(Mode mode, T value0, T value1, float chance)
  : _mode(mode)
  , _value0(value0)
  , _value1(value1)
  , _chance(chance) { }

  T get() const {
    switch (_mode) {
      case Mode::VALUE:
        return _value0;
      case Mode::RANDOM_RANGE:
        return getRandomValue();
      case Mode::CHANCE:
        return getChanceValue();
      case Mode::NONE:
      default:
        return T();
    }
  }

  void readJson(const ofJson& obj) override {
    if (!obj.is_null()) {
      ofxTCommon::JsonUtil::assertIsObject(obj);
      const auto& val = obj["value"];
      if (!val.is_null()) {
        _mode = Mode::VALUE;
        _value0 = ofxTCommon::JsonUtil::fromJson<T>(val);
        _value1 = _value0;
        _chance = 0;
        return;
      }
      const auto& range = obj["range"];
      if (!range.is_null()) {
        ofxTCommon::JsonUtil::assertIsArray(range, 2);
        _mode = Mode::RANDOM_RANGE;
        _value0 = ofxTCommon::JsonUtil::fromJson<T>(range[0]);
        _value1 = ofxTCommon::JsonUtil::fromJson<T>(range[1]);
        _chance = 0;
        return;
      }
      const auto& chance = obj["chance"];
      const auto& options = obj["options"];
      if (!chance.is_null() && !options.is_null()) {
        ofxTCommon::JsonUtil::assertIsArray(options, 2);
        _mode = Mode::CHANCE;
        _value0 = ofxTCommon::JsonUtil::fromJson<T>(options[0]);
        _value1 = ofxTCommon::JsonUtil::fromJson<T>(options[1]);
        _chance = chance;
        return;
      }
    }
    _mode = Mode::NONE;
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
          {"range", { _value0, _value1 }},
        };
      case Mode::CHANCE:
        return {
          {"chance", _chance},
          {"options", { _value0, _value1}},
        };
      case Mode::NONE:
      default:
        return nullptr;
    }
  }

  std::string typeName() const override { return "SceneValue"; }
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

namespace SceneValues {
  template<typename T>
  SceneValue<T> createValue(T value) {
    return SceneValue<T>(SceneValueMode::VALUE, value, value, 0);
  }
  template<typename T>
  SceneValue<T> createRandomRange(T value0, T value1) {
    return SceneValue<T>(SceneValueMode::RANDOM_RANGE,
                         value0, value1, 0);
  }
  template<typename T>
  SceneValue<T> createChance(T value0, T value1, float chance) {
    return SceneValue<T>(SceneValueMode::CHANCE,
                         value0, value1, chance);
  }

  template<typename T>
  SceneValue<T> createNone() {
    return SceneValue<T>(SceneValueMode::NONE, T(), T(), 0);
  }
}
