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
  ofxDatGuiToggle* toggle = _folder->addToggle(param.getName());
  toggle->onToggleEvent(this, &AbstractParamsGui::onToggleEvent);
  return toggle;
}

void AbstractParamsGui::onToggleEvent(ofxDatGuiToggleEvent e) {
  ofParameter<bool>& param = _baseParams.getBool(e.target->getName());
  param.set(e.checked);
}
