//
//  GuiControls.cpp
//  memory
//
//  Created by tekt on 8/16/16.
//
//

#include <stdexcept>
#include <typeinfo>
#include "GuiControls.h"

ofxBaseGui* TGuiBuilder::createControl(const TParamBase &param) {
  const auto& type = param.getTypeInfo();
  if (param.isGroup()) {
    if (type == typeid(ValueRange<float>)) {
      return createRangeSliders(dynamic_cast<const ValueRange<float>&>(param));
    } else if (type == typeid(ValueRange<int>)) {
      return createRangeSliders(dynamic_cast<const ValueRange<int>&>(param));
    } else {
      return createGroup(dynamic_cast<const Params&>(param));
    }
  }
  if (type == typeid(bool)) {
    return createToggle(dynamic_cast<const TParam<bool>&>(param));
  } else if (type == typeid(float)) {
    return createNumberSlider(dynamic_cast<const TParam<float>&>(param));
  } else if (type == typeid(int)) {
    return createNumberSlider(dynamic_cast<const TParam<int>&>(param));
  } else if (type == typeid(ofVec3f)) {
    return createVectorSliders(dynamic_cast<const TParam<ofVec3f>&>(param));
  } else if (type == typeid(ofFloatColor)) {
    return createColorSliders(dynamic_cast<const TParam<ofFloatColor>&>(param));
  } else {
    throw std::invalid_argument(std::string("Unsupported parameter type: ") + type.name());
  }
}

ofxGuiGroupExtended* TGuiBuilder::createGroup(const Params &params) {
  //...
}

ofxBaseGui* TGuiBuilder::createToggle(const TParam<bool> &param) {
  //...
}

ofxBaseGui* TGuiBuilder::createNumberSlider(const TParam<float> &param) {
  //...
}

ofxBaseGui* TGuiBuilder::createNumberSlider(const TParam<int> &param) {
  //...
}

ofxBaseGui* TGuiBuilder::createVectorSliders(const TParam<ofVec3f> &param) {
  //...
}

ofxBaseGui* TGuiBuilder::createColorSliders(const TParam<ofFloatColor> &param) {
  //...
}

ofxBaseGui* TGuiBuilder::createRangeSliders(const ValueRange<float> &params) {
  //...
}

ofxBaseGui* TGuiBuilder::createRangeSliders(const ValueRange<int> &params) {
  //...
}
