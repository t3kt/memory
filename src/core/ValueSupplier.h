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

class SimpleRandomVectorSupplier : public FloatValueRange {
public:
  explicit SimpleRandomVectorSupplier(std::string name)
  : FloatValueRange(name) {}

  SimpleRandomVectorSupplier& set(float minVal, float maxVal) {
    ValueRange<float>::set(minVal, maxVal);
    return *this;
  }
  SimpleRandomVectorSupplier& setParamRange(float minVal, float maxVal) {
    ValueRange<float>::setParamRange(minVal, maxVal);
    return *this;
  }

  ofVec3f getValue() const;
};

class RandomHsbFloatColorSupplier : public Params {
public:
  explicit RandomHsbFloatColorSupplier(std::string name);

  Json to_json() const override;
  void read_json(const Json& obj) override;

  RandomHsbFloatColorSupplier& setHue(float low, float high) {
    hueRange.set(low, high);
    return *this;
  }
  
  RandomHsbFloatColorSupplier& setSaturation(float low, float high) {
    saturationRange.set(low, high);
    return *this;
  }
  
  RandomHsbFloatColorSupplier& setBrightness(float low, float high) {
    brightnessRange.set(low, high);
    return *this;
  }
  
  RandomHsbFloatColorSupplier& setAlpha(float low, float high) {
    alphaRange.set(low, high);
    return *this;
  }
  
  ofFloatColor getValue() const;

  RandomValueSupplier<float> hueRange;
  RandomValueSupplier<float> saturationRange;
  RandomValueSupplier<float> brightnessRange;
  RandomValueSupplier<float> alphaRange;
};

#endif /* ValueSpecifier_h */
