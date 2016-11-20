//
//  ControlValue.cpp
//

#include <cmath>
#include <ofVec3f.h>
#include "../control/ControlValue.h"

using namespace ofxTCommon;

EnumTypeInfo<ControlValueMode> ControlValueModeInfo {
  {"unspecified", ControlValueMode::UNSPECIFIED},
  {"value", ControlValueMode::VALUE},
  {"randomRange", ControlValueMode::RANDOM_RANGE},
  {"chance", ControlValueMode::CHANCE},
};

template<>
const EnumTypeInfo<ControlValueMode>& ofxTCommon::getEnumInfo() {
  return ControlValueModeInfo;
}

std::ostream& operator<<(std::ostream& os,
                         const ControlValueMode& mode) {
  os << enumToString(mode);
}

template<>
float ControlValue<float>::getRandomValue() const {
  return ofRandom(_value0, _value1);
}

template<>
int ControlValue<int>::getRandomValue() const {
  float value = ofRandom(_value0, _value1);
  return static_cast<int>(std::round(value));
}

template<>
ofVec3f ControlValue<ofVec3f>::getRandomValue() const {
  return ofVec3f(ofRandom(_value0.x, _value1.x),
                 ofRandom(_value0.y, _value1.y),
                 ofRandom(_value0.z, _value1.z));
}

template<>
bool ControlValue<bool>::getRandomValue() const {
  return getChanceValue();
}
