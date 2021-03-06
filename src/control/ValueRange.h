//
//  ValueRange.h
//

#pragma once

#include "../control/Params.h"

// Parameters that specify a range of values of type T.
template<typename T>
class ValueRange : public Params {
public:
  ValueRange()
  : Params() {
    add(lowValue
        .setKey("low")
        .setName("Low"));
    add(highValue
        .setKey("high")
        .setName("High"));
  }

  ValueRange<T>& setKey(std::string key) {
    Params::setKey(key);
    return *this;
  }

  ValueRange<T>& setName(std::string name) {
    Params::setName(name);
    return *this;
  }

  ValueRange<T>& setParamKeys(std::string lowKey, std::string highKey) {
    lowValue.setKey(lowKey);
    highValue.setKey(highKey);
    return *this;
  }

  ValueRange<T>& setParamNames(std::string lowName, std::string highName) {
    lowValue.setName(lowName);
    highValue.setName(highName);
    return *this;
  }

  ValueRange<T>& setParamValues(T low, T high) {
    lowValue.set(low);
    highValue.set(high);
    return *this;
  }

  ValueRange<T>& setParamValuesAndDefaults(T low, T high) {
    lowValue.setValueAndDefault(low);
    highValue.setValueAndDefault(high);
    return *this;
  }

  ValueRange<T>& setParamRanges(T minVal, T maxVal) {
    lowValue.setMin(minVal);
    lowValue.setMax(maxVal);
    highValue.setMin(minVal);
    highValue.setMax(maxVal);
    return *this;
  }

  TParam<T> lowValue;
  TParam<T> highValue;
};
