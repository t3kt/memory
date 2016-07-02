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
#include "JsonIO.h"

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

  // json11 library requires this naming
  virtual Json to_json() const {
    return nullptr;
  }

  virtual void read_json(const Json& obj) { }
  
  virtual void initPanel(ofxGuiGroup& panel) {}

  ParamsGui* getGui() {
    if (_gui == nullptr) {
      _gui = createGui();
      if (_gui != nullptr) {
        _gui->setup();
      }
    }
    return _gui;
  }
protected:
  virtual ParamsGui* createGui() { return nullptr; }
private:
  ParamsGui* _gui;
};

template<typename T>
class ValueRange : public Params {
public:
  explicit ValueRange(std::string name)
  : Params(name) {
    lowValue.setName("Low");
    highValue.setName("High");
    add(lowValue);
    add(highValue);
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

  Json to_json() const override {
    return Json::array { lowValue.get(), highValue.get() };
  }
  
  ofParameter<T> lowValue;
  ofParameter<T> highValue;

protected:
  ParamsGui* createGui() override;
};

using FloatValueRange = ValueRange<float>;

#endif /* Params_h */
