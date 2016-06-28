//
//  Params.h
//  memory-prototype-2
//
//  Created by tekt on 6/24/16.
//
//

#ifndef Params_h
#define Params_h

#include <string>
#include <ofParameterGroup.h>
#include <ofxGuiGroup.h>
#include "Common.h"

class ofxDatGuiFolder;

class ParamsGui {
public:
  virtual void setup() {}
  virtual ofxDatGuiFolder* getGuiFolder() = 0;
};

class Params : public ofParameterGroup {
public:
  Params() {}
  explicit Params(std::string label) {
    setName(label);
  }
  
  virtual void initPanel(ofxGuiGroup& panel) {}
  virtual ParamsGui* getGui() { return nullptr; }
};

template<typename T>
class ValueRange : public Params {
public:
  explicit ValueRange(std::string name)
  : Params(name) {
    init();
  }
  
  ValueRange() {
    init();
  }

  ValueRange<T>& setNames(std::string lowName, std::string highName) {
    lowValue.setName(lowName);
    highValue.setName(highName);
    return *this;
  }

  ValueRange<T>& set(T low, T high) {
    lowValue.set(low);
    highValue.set(high);
    return *this;
  }

  ValueRange<T>& setParamRange(T minVal, T maxVal) {
    lowValue.setMin(minVal);
    lowValue.setMax(maxVal);
    highValue.setMin(minVal);
    highValue.setMax(maxVal);
    return *this;
  }

  T getLerped(float amount) const {
    return getInterpolated(lowValue.get(), highValue.get(), amount);
  }
  
  ofParameter<T> lowValue;
  ofParameter<T> highValue;

private:
  void init() {
    lowValue.setName("Low");
    highValue.setName("High");
    add(lowValue);
    add(highValue);
  }
};

#endif /* Params_h */
