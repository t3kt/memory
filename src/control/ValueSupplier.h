//
//  ValueSupplier.h
//

#pragma once

#include <ofParameter.h>
#include <ofParameterGroup.h>
#include <ofTypes.h>
#include <string>
#include "../control/Params.h"
#include "../control/ValueRange.h"

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

class RandomBoolSupplier
: public Params {
public:
  RandomBoolSupplier() {
    add(chance
        .setKey("chance")
        .setName("Chance")
        .setRange(0, 1)
        .setValueAndDefault(0.5));
  }

  RandomBoolSupplier& setKey(std::string key) {
    Params::setKey(key);
    return *this;
  }

  RandomBoolSupplier& setName(std::string name) {
    Params::setName(name);
    return *this;
  }

  RandomBoolSupplier& setChanceValueAndDefault(float chanceVal) {
    chance.setValueAndDefault(chanceVal);
    return *this;
  }

  bool getValue() const;

  TParam<float> chance;
};

class SimpleRandomVectorSupplier : public ValueRange<float> {
public:
  SimpleRandomVectorSupplier& set(float minVal, float maxVal) {
    ValueRange<float>::setParamValues(minVal, maxVal);
    return *this;
  }
  SimpleRandomVectorSupplier& setParamRanges(float minVal, float maxVal) {
    ValueRange<float>::setParamRanges(minVal, maxVal);
    return *this;
  }

  ofVec3f getValue() const;
};
