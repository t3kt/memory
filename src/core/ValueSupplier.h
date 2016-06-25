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

class AbstractValueSupplier : public ofParameterGroup {
public:
  AbstractValueSupplier(std::string name) {
    setName(name);
  };
};

template <typename T>
class RandomValueSupplier : public AbstractValueSupplier {
public:
  RandomValueSupplier(std::string name)
  : AbstractValueSupplier(name) {
    _lowValue.setName("Low");
    _highValue.setName("High");
    add(_lowValue, _highValue);
  };
  
  RandomValueSupplier& set(T minVal, T maxVal) {
    _lowValue.set(minVal);
    _highValue.set(maxVal);
    return *this;
  };
  
  RandomValueSupplier& setLow(T low) {
    _lowValue.set(low);
    return *this;
  };
  
  RandomValueSupplier& setHigh(T high) {
    _highValue.set(high);
    return *this;
  };
  
  RandomValueSupplier& setParamRange(T minVal, T maxVal) {
    _lowValue.setMin(minVal);
    _lowValue.setMax(maxVal);
    _highValue.setMin(minVal);
    _highValue.setMax(maxVal);
    return *this;
  };
  
  T getValue() const;
  
private:
  ofParameter<T> _lowValue;
  ofParameter<T> _highValue;
};

class FloatColorSupplier : public AbstractValueSupplier {
public:
  FloatColorSupplier(std::string name) : AbstractValueSupplier(name) { };
  
  virtual ofFloatColor getValue() const = 0;
};

class RandomHsbFloatColorSupplier : public FloatColorSupplier {
public:
  RandomHsbFloatColorSupplier(std::string name);
  
  RandomHsbFloatColorSupplier& setHue(float low, float high) {
    _hueRange.setLow(low).setHigh(high);
    return *this;
  }
  
  RandomHsbFloatColorSupplier& setSaturation(float low, float high) {
    _saturationRange.setLow(low).setHigh(high);
    return *this;
  }
  
  RandomHsbFloatColorSupplier& setBrightness(float low, float high) {
    _brightnessRange.setLow(low).setHigh(high);
    return *this;
  }
  
  RandomHsbFloatColorSupplier& setAlpha(float low, float high) {
    _alphaRange.setLow(low).setHigh(high);
    return *this;
  }
  
  ofFloatColor getValue() const override;
  
private:
  RandomValueSupplier<float> _hueRange;
  RandomValueSupplier<float> _saturationRange;
  RandomValueSupplier<float> _brightnessRange;
  RandomValueSupplier<float> _alphaRange;
};

#endif /* ValueSpecifier_h */
