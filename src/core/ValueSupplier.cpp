//
//  ValueSupplier.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/24/16.
//
//

#include "ValueSupplier.h"
#include <ofMain.h>
#include <cmath>

template<>
float RandomValueSupplier<float>::getValue() const {
  return ofRandom(_lowValue.get(), _highValue.get());
}

template<>
int RandomValueSupplier<int>::getValue() const {
  float value = ofRandom(_lowValue.get(), _highValue.get());
  return static_cast<int>(std::round(value));
}

template<>
uint8 RandomValueSupplier<uint8>::getValue() const {
  float value = ofRandom(_lowValue.get(), _highValue.get());
  return static_cast<uint8>(std::round(value));
}

template<>
ofVec3f RandomValueSupplier<ofVec3f>::getValue() const {
  ofVec3f minVal = _lowValue.get();
  ofVec3f maxVal = _highValue.get();
  return ofVec3f(ofRandom(minVal.x, maxVal.x),
                 ofRandom(minVal.y, maxVal.y),
                 ofRandom(minVal.z, maxVal.z));
}

RandomHsbFloatColorSupplier::RandomHsbFloatColorSupplier(std::string name)
: FloatColorSupplier(name)
, _hueRange("Hue")
, _saturationRange("Saturation")
, _brightnessRange("Brightness")
, _alphaRange("Alpha") {
  
  add(_hueRange.setParamRange(0, 1).setLow(0).setHigh(1),
      _saturationRange.setParamRange(0, 1).setLow(0).setHigh(1),
      _brightnessRange.setParamRange(0, 1).setLow(0).setHigh(1),
      _alphaRange.setParamRange(0, 1).setLow(1).setHigh(1));
}

ofFloatColor RandomHsbFloatColorSupplier::getValue() const {
  return ofFloatColor::fromHsb(_hueRange.getValue(),
                               _saturationRange.getValue(),
                               _brightnessRange.getValue(),
                               _alphaRange.getValue());
}
