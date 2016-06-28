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
  FloatValueRangeGui(FloatValueRange& params)
  : AbstractParamsGui(params)
  , _params(params) { }
protected:
  void addControls() override;
private:
  FloatValueRange& _params;
};

void FloatValueRangeGui::addControls() {
  addSlider(_params.lowValue);
  addSlider(_params.highValue);
}

ParamsGui* FloatValueRange::createGui() {
  return new FloatValueRangeGui(*this);
}

