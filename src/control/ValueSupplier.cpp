//
//  ValueSupplier.cpp
//

#include <cmath>
#include <ofMain.h>
#include "../control/ValueSupplier.h"

template<>
float RandomValueSupplier<float>::getValue() const {
  return ofRandom(lowValue(), highValue());
}

template<>
int RandomValueSupplier<int>::getValue() const {
  float value = ofRandom(lowValue(), highValue());
  return static_cast<int>(std::round(value));
}

template<>
ofVec3f RandomValueSupplier<ofVec3f>::getValue() const {
  ofVec3f minVal = lowValue();
  ofVec3f maxVal = highValue();
  return ofVec3f(ofRandom(minVal.x, maxVal.x),
                 ofRandom(minVal.y, maxVal.y),
                 ofRandom(minVal.z, maxVal.z));
}

bool RandomBoolSupplier::getValue() const {
  float val = ofRandomuf();
  return val < chance.get();
}

ofVec3f SimpleRandomVectorSupplier::getValue() const {
  float dist = ofRandom(lowValue(), highValue());
  ofVec3f value = ofVec3f(dist, 0, 0);
  value.rotate(ofRandom(0, 360),
               ofRandom(0, 360),
               ofRandom(0, 360));
  return value;
}
