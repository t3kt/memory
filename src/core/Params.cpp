//
//  Params.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/28/16.
//
//

#include "Params.h"
#include "ParamsGui.h"

class FloatValueRangeGui
: public AbstractParamsGui {
public:
  FloatValueRangeGui(ValueRange<float>& params)
  : AbstractParamsGui(params)
  , _params(params) { }
protected:
  void addControls() override {
    addSlider(_params.lowValue);
    addSlider(_params.highValue);
  }
private:
  ValueRange<float>& _params;
};

template<>
ParamsGui* ValueRange<float>::createGui() {
  return new FloatValueRangeGui(*this);
}

class FloatColorValueRangeGui
: public AbstractParamsGui {
public:
  FloatColorValueRangeGui(ValueRange<ofFloatColor>& params)
  : AbstractParamsGui(params)
  , _params(params) { }
protected:
  void addControls() override {
    addColorPicker(_params.lowValue);
    addColorPicker(_params.highValue);
  }
private:
  ValueRange<ofFloatColor>& _params;
};

template<>
ParamsGui* ValueRange<ofFloatColor>::createGui() {
  return new FloatColorValueRangeGui(*this);
}

