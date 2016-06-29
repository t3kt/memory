//
//  ParamsGui.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/28/16.
//
//

#include "ParamsGui.h"

AbstractParamsGui::AbstractParamsGui(Params& params)
: _baseParams(params) {
}

void AbstractParamsGui::setup() {
  _folder = new ofxDatGuiFolder(_baseParams.getName());
  addControls();
}

ofxDatGuiToggle* AbstractParamsGui::addToggle(ofParameter<bool> &param) {
  ofxDatGuiToggle* toggle = _folder->addToggle(param.getName(),
                                               param.get());
  toggle->onToggleEvent(this, &AbstractParamsGui::onToggleEvent);
  return toggle;
}

void AbstractParamsGui::onToggleEvent(ofxDatGuiToggleEvent e) {
  ofParameter<bool>& param = _baseParams.getBool(e.target->getName());
  param.set(e.checked);
}

ofxDatGuiSlider* AbstractParamsGui::addSlider(ofParameter<float> &param) {
  ofxDatGuiSlider* slider = _folder->addSlider(param.getName(),
                                               param.getMin(),
                                               param.getMax(),
                                               param.get());
  slider->onSliderEvent(this, &AbstractParamsGui::onSliderEvent);
  return slider;
}

void AbstractParamsGui::onSliderEvent(ofxDatGuiSliderEvent e) {
  ofParameter<float>& param = _baseParams.getFloat(e.target->getName());
  param.set(e.value);
}

ofxDatGuiColorPicker* AbstractParamsGui::addColorPicker(ofParameter<ofFloatColor> &param) {
  ofxDatGuiColorPicker* picker = _folder->addColorPicker(param.getName());
  picker->setColor(param.get());
  return picker;
}

void AbstractParamsGui::onColorPickerEvent(ofxDatGuiColorPickerEvent e) {
  ofParameter<ofFloatColor>& param = _baseParams.getFloatColor(e.target->getName());
  param.set(e.color);
}
