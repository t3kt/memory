//
//  ParamsGui.cpp
//  memory
//
//  Created by tekt on 7/19/16.
//
//

#include <ofLog.h>
#include <stdexcept>
#include "ParamsGui.h"

std::shared_ptr<ParamGui> createParamGuiControl(TParamBase& param) {
  const auto& type = param.getTypeInfo();
  if (type == typeid(bool)) {
    return std::make_shared<ParamGuiToggle>(dynamic_cast<TParam<bool>&>(param));
  } else if (type == typeid(float)) {
    return std::make_shared<ParamGuiSlider<float>>(dynamic_cast<TParam<float>&>(param));
  } else if (type == typeid(int)) {
    return std::make_shared<ParamGuiSlider<int>>(dynamic_cast<TParam<int>&>(param));
  }
  ofLogWarning() << "Unsupported param type: " << type.name();
  return std::shared_ptr<ParamGui>();
}

void ParamGuiToggle::addToParent(ofxControlWidget *parent) {
  _toggle =
  parent->addToggle(_param.getName(),
                    &_storage,
                    this,
                    &ParamGuiToggle::onToggleEvent);
}

template<typename T>
void ParamGuiSlider<T>::addToParent(ofxControlWidget *parent) {
  const auto& param = AbstractParamGuiControl<T>::_param;
  _slider =
  parent->addSlider(param.getName(),
                    &_storage,
                    param.getMin(),
                    param.getMax(),
                    this,
                    &ParamGuiSlider<T>::onSliderEvent);
}

void AbstractParamsGui::addControlsForParams(Params& params) {
  for (auto param : params.paramBases()) {
    auto ctrl = createParamGuiControl(*param);
    if (ctrl) {
      ctrl->addToParent(&_root);
      _controls.push_back(ctrl);
    }
  }
}

void AbstractParamsGui::addToParent(ofxControlWidget* parent) {
  parent->addWidget(&_root);
  build();
}
