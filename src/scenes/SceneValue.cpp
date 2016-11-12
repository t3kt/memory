//
//  SceneValue.cpp
//

#include <cmath>
#include <ofVec3f.h>
#include "../scenes/SceneValue.h"

template<>
float SceneValue<float>::getRandomValue() const {
  return ofRandom(_value0, _value1);
}

template<>
int SceneValue<int>::getRandomValue() const {
  float value = ofRandom(_value0, _value1);
  return static_cast<int>(std::round(value));
}

template<>
ofVec3f SceneValue<ofVec3f>::getRandomValue() const {
  return ofVec3f(ofRandom(_value0.x, _value1.x),
                 ofRandom(_value0.y, _value1.y),
                 ofRandom(_value0.z, _value1.z));
}

template<>
bool SceneValue<bool>::getRandomValue() const {
  return getChanceValue();
}
