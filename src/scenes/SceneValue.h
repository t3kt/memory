//
//  SceneValue.h
//  memory
//

#pragma once

#include <ofMath.h>

enum class SceneValueMode {
  VALUE,
  RANDOM_RANGE,
  CHANCE,
};

template<typename T>
class SceneValue {
public:
  using Mode = SceneValueMode;

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
      default:
        return T();
    }
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
}
