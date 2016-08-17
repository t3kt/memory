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

ofxBaseGui* TGuiBuilder::createControl(const TParamBase &param,
                                       bool isChild) const {
  if (isChild
      && _ignoreKeys.find(param.getKey()) != _ignoreKeys.end()) {
    return nullptr;
  }
  const auto& type = param.getTypeInfo();
  if (param.isGroup()) {
    if (type == typeid(ValueRange<float>)) {
      return createRangeSliders(dynamic_cast<const ValueRange<float>&>(param));
    } else if (type == typeid(ValueRange<int>)) {
      return createRangeSliders(dynamic_cast<const ValueRange<int>&>(param));
    } else if (!isChild) {
      return createGroup(dynamic_cast<const Params&>(param));
    } else {
      return createChildGroup(dynamic_cast<const Params&>(param));
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

ofxBaseGui* TGuiBuilder::createChildGroup(const Params &params) const {
  const auto& builder = getSubBuilder(params);
  return builder.createControl(params, false);
}

ofxBaseGui* TGuiBuilder::createGroup(const Params &params) const {
  ofxGuiGroupExtended* group;
  switch (_groupMode) {
    case GuiGroupMode::GROUP:
      group = (new ofxGuiGroupExtended())->setup(params.getName());
      break;
    case GuiGroupMode::PAGE:
      group = (new ofxGuiPage())->setup(params.getName());
      break;
    case GuiGroupMode::PANEL:
      group = (new ofxPanelExtended())->setup(params.getName());
      break;
    case GuiGroupMode::TABBED_PAGES:
      // TODO: implement tabbed pages support
      return nullptr;
    default:
      return nullptr;
  }
  for (const auto param : params.paramBases()) {
    auto control = createControl(*param, true);
    if (control) {
      group->add(control);
    }
  }
  return group;
}

ofxBaseGui* TGuiBuilder::createToggle(const TParam<bool> &param) const {
  return new ofxMinimalToggle(param);
}


template<typename T>
ofxBaseGui* TGuiBuilder::createNumberSlider(const TParam<T> &param) const {
  auto style = getSliderStyle(param);
  switch (style) {
    case GuiSliderStyle::ROTARY_SLIDER:
      return new ofxRotarySlider<T>(param);
    case GuiSliderStyle::VERTICAL_SLIDER:
      return new ofxVerticalSlider<T>(param);
    default:
      return new ofxSlider<T>(param);
  }
}

ofxBaseGui* TGuiBuilder::createVectorSliders(const TParam<ofVec3f> &param) const {
  return new ofxVec3Slider(param);
}

ofxBaseGui* TGuiBuilder::createColorSliders(const TParam<ofFloatColor> &param) const {
  return new ofxFloatColorSlider(param);
}

ofxBaseGui* TGuiBuilder::createRangeSliders(const ValueRange<float> &params) const {
  return new ofxGuiGroupExtended(params);
}

ofxBaseGui* TGuiBuilder::createRangeSliders(const ValueRange<int> &params) const {
  return new ofxGuiGroupExtended(params);
}
