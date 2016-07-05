//
//  ValueSupplier.h
//  memory-prototype-2
//
//  Created by tekt on 6/24/16.
//
//

#ifndef ValueSupplier_h
#define ValueSupplier_h

#include <ofParameter.h>
#include <ofParameterGroup.h>
#include <ofTypes.h>
#include <string>
#include "Params.h"

template <typename T>
class RandomValueSupplier : public ValueRange<T> {
public:
  RandomValueSupplier& setParamValues(T minVal, T maxVal) {
    ValueRange<T>::setParamValues(minVal, maxVal);
    return *this;
  };
  
  RandomValueSupplier& setParamRanges(T minVal, T maxVal) {
    ValueRange<T>::setParamRanges(minVal, maxVal);
    return *this;
  };
  
  T getValue() const;
};

class SimpleRandomVectorSupplier : public FloatValueRange {
public:
  SimpleRandomVectorSupplier& set(float minVal, float maxVal) {
    FloatValueRange::setParamValues(minVal, maxVal);
    return *this;
  }
  SimpleRandomVectorSupplier& setParamRanges(float minVal, float maxVal) {
    FloatValueRange::setParamRanges(minVal, maxVal);
    return *this;
  }

  ofVec3f getValue() const;
};

class RandomHsbFloatColorSupplier : public Params {
public:
  RandomHsbFloatColorSupplier();

  RandomHsbFloatColorSupplier& setHue(float low, float high) {
    hueRange.setParamValues(low, high);
    return *this;
  }
  
  RandomHsbFloatColorSupplier& setSaturation(float low, float high) {
    saturationRange.setParamValues(low, high);
    return *this;
  }
  
  RandomHsbFloatColorSupplier& setBrightness(float low, float high) {
    brightnessRange.setParamValues(low, high);
    return *this;
  }
  
  RandomHsbFloatColorSupplier& setAlpha(float low, float high) {
    alphaRange.setParamValues(low, high);
    return *this;
  }
  
  ofFloatColor getValue() const;

  RandomValueSupplier<float> hueRange;
  RandomValueSupplier<float> saturationRange;
  RandomValueSupplier<float> brightnessRange;
  RandomValueSupplier<float> alphaRange;
};

#endif /* ValueSpecifier_h */
