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

template <typename T>
class RandomValueSupplier {
public:
  RandomValueSupplier(std::string name) {
    paramGroup.setName(name);
    minValue.setName("Min");
    maxValue.setName("Max");
    paramGroup.add(minValue);
    paramGroup.add(maxValue);
  };
  
  RandomValueSupplier& set(T minVal, T maxVal) {
    minValue.set(minVal);
    maxValue.set(maxVal);
    return *this;
  };
  
  RandomValueSupplier& setParamRange(T minVal, T maxVal) {
    minValue.setMin(minVal);
    minValue.setMax(maxVal);
    maxValue.setMin(minVal);
    maxValue.setMax(maxVal);
    return *this;
  };
  
  T getValue() const;
  
  operator ofParameterGroup&() { return paramGroup; };
  operator const ofParameterGroup&() const { return paramGroup; };
  
  ofParameterGroup paramGroup;
  ofParameter<T> minValue;
  ofParameter<T> maxValue;
};

#endif /* ValueSpecifier_h */
