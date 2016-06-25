//
//  ValueSupplier.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/24/16.
//
//

#include "ValueSupplier.h"
#include <ofMain.h>

template<>
float RandomValueSupplier<float>::getValue() const {
  return ofRandom(minValue, maxValue);
}

template<>
ofVec3f RandomValueSupplier<ofVec3f>::getValue() const {
  ofVec3f minVal = minValue.get();
  ofVec3f maxVal = maxValue.get();
  return ofVec3f(ofRandom(minVal.x, maxVal.x),
                 ofRandom(minVal.y, maxVal.y),
                 ofRandom(minVal.z, maxVal.z));
}
