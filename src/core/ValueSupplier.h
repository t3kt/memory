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
  explicit RandomValueSupplier(std::string name)
  : ValueRange<T>(name) {};
  
  RandomValueSupplier& set(T minVal, T maxVal) {
    ValueRange<T>::set(minVal, maxVal);
    return *this;
  };
  
  RandomValueSupplier& setParamRange(T minVal, T maxVal) {
    ValueRange<T>::setParamRange(minVal, maxVal);
    return *this;
  };
  
  T getValue() const;
};

class RandomHsbFloatColorSupplier : public Params {
public:
  explicit RandomHsbFloatColorSupplier(std::string name);
  
  RandomHsbFloatColorSupplier& setHue(float low, float high) {
    _hueRange.set(low, high);
    return *this;
  }
  
  RandomHsbFloatColorSupplier& setSaturation(float low, float high) {
    _saturationRange.set(low, high);
    return *this;
  }
  
  RandomHsbFloatColorSupplier& setBrightness(float low, float high) {
    _brightnessRange.set(low, high);
    return *this;
  }
  
  RandomHsbFloatColorSupplier& setAlpha(float low, float high) {
    _alphaRange.set(low, high);
    return *this;
  }
  
  ofFloatColor getValue() const;
  
private:
  RandomValueSupplier<float> _hueRange;
  RandomValueSupplier<float> _saturationRange;
  RandomValueSupplier<float> _brightnessRange;
  RandomValueSupplier<float> _alphaRange;
};

#endif /* ValueSpecifier_h */
